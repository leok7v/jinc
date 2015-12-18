/*  Copyright (c) 2003-2013, Leo Kuznetsov
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.

	* Neither the name of the {organization} nor the names of its
	  contributors may be used to endorse or promote products derived from
	  this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifdef map_t

enum { EMPTY = 0, REMOVED = -1 };

typedef struct _map_s {
	int capacity;
	int size;
	int* hashes;
	key_t*   keys;
	value_t* values;
	boolean can_grow;
	boolean allocated;
} _map_t;

void map_dispose(map_t map);
static boolean put_hashed(_map_t* map, key_t k, value_t v, int hash, boolean dup);

static void dispose(_map_t* m) { /* free the tables w/o cleaning them */
	if (m != null) {
		free(m->keys);
		m->keys = null;
		free(m->values);
		m->values = null;
		free(m->hashes);
		m->hashes = null;
	}
}

map_t map_create(int capacity, boolean can_grow) {
	capacity = capacity < 16 ? 16 : capacity;
	_map_t* map = (_map_t*)calloc(1, sizeof(_map_t));
	if (map != null) {
		map->keys   = (key_t*)calloc(capacity, sizeof(key_t));
		map->values = (value_t*)calloc(capacity, sizeof(value_t));
		map->hashes = (int*)calloc(capacity, sizeof(int));
		map->capacity = capacity;
		map->can_grow = can_grow;
		map->allocated = true;
		if (map->keys == null || map->values == null || map->hashes == null) {
			map_dispose(map);
			map = null;
		}
	}
	return (map_t)map;
}

map_t map_init_static(void* address, int bytes) {
	int capacity = (bytes - sizeof(_map_t)) / (sizeof(key_t) + sizeof(value_t) + sizeof(int));
	assert(capacity >= 16);
	if (capacity < 16) {
		return null;
	}
	char* a = (char*)address;
	_map_t* map = (_map_t*)a; a += sizeof(_map_t);
	if (map != null) {
		map->keys   = (key_t*)a;   a += capacity * sizeof(key_t);
		map->values = (value_t*)a; a += capacity * sizeof(value_t);
		map->hashes = (int*)a;     a += capacity * sizeof(int);
		map->capacity = capacity;
		map->can_grow = false;
		map->allocated = false;
        assert(a <= (char*)address + bytes); (void)a;
	}
	return (map_t)map;
}

int map_size(const map_t m) {
	_map_t* map = (_map_t*)m;
	return map->size;
}

static boolean grow(_map_t* m1) {
	assert(m1->can_grow);
	int c = m1->capacity;
	_map_t* m2 = (_map_t*)map_create(c * 3 / 2, true);
	assert(m2 != null);
	if (m2 != null) {
		int i = 0;
		while (i < c) {
			if (m1->keys[i] != 0 && m1->hashes[i] != REMOVED) {
				boolean b = put_hashed(m2, m1->keys[i], m1->values[i], m1->hashes[i], false);
				if (!b) {
					map_dispose(m2);
					return false;
				}
				m1->hashes[i] = EMPTY;
				m1->keys[i] = key_null;
				m1->values[i] = value_null;
			}
			i++;
		}
	}
	assert(m2->size == m1->size);
	dispose(m1);
	m1->keys = m2->keys;
	m1->values = m2->values;
	m1->hashes = m2->hashes;
	m1->capacity = m2->capacity;
	m2->keys = null;
	m2->values = null;
	m2->hashes = null;
	map_dispose(m2);
	return true;
}

boolean map_put(map_t map, key_t k, value_t v) {
	_map_t* m = (_map_t*)map;
	assert(k != key_null && v != value_null);
	if (k != key_null && v != value_null) {
		int h = hashcode(k);
		assert(h > 0);
		boolean b = put_hashed(m, k, v, h, true);
		b = b || (m->can_grow && (grow(m) && put_hashed(m, k, v, h, true)));
		return b;
	} else {
		return false;
	}
}

static boolean put_hashed(_map_t* m, key_t k, value_t v, int hash, boolean dup) {
	assert(hash > 0);
	int h = hash % m->capacity;
	int h0 = h;
	for (;;) {
		if (m->hashes[h] == EMPTY || m->hashes[h] == REMOVED || key_equ(m->keys[h], k)) {
			break;
		}
		h = (h + 1) % m->capacity;
		if (h == h0) {
			return false; /* need to grow */
		}
	}
	if (m->hashes[h] == EMPTY || m->hashes[h] == REMOVED) {
		if (dup) {
			k = key_dup(k);
			if (k == key_null) { return false; }
			v = value_dup(v);
			if (v == value_null) { key_free(k); return false; }
		}
		m->keys[h] = k;
		m->hashes[h] = hash;
		m->size++;
		m->values[h] = v;
	} else {
		assert(key_equ(k, m->keys[h]));
		if (dup) {
			v = value_dup(v);
			if (v == value_null) { return false; }
		}
		value_free(m->values[h]);
		m->values[h] = v;
	}
	return true;
}

value_t map_get(const map_t map, key_t k) { /* does not "dup" result! */
	_map_t* m = (_map_t*)map;
	int hash = hashcode(k);
	int h = hash % m->capacity;
	int h0 = h;
	for (;;) {
		if (m->hashes[h] == EMPTY) {
			return value_null;
		}
		if (key_equ(m->keys[h], k)) {
			return m->values[h];
		}
		h = (h + 1) % m->capacity;
		if (h == h0) {
			return value_null;
		}
	}
}

void map_remove(map_t map, key_t k) {
	_map_t* m = (_map_t*)map;
	int hash = hashcode(k);
	int h = hash % m->capacity;
	int h0 = h;
	for (;;) {
		if (m->hashes[h] == EMPTY) {
			return;
		}
		if (key_equ(m->keys[h], k)) {
			m->hashes[h] = REMOVED;
			m->size--;
			key_free(m->keys[h]); m->keys[h] = key_null;
			value_free(m->values[h]); m->values[h] = value_null;
		}
		h = (h + 1) % m->capacity;
		if (h == h0) {
			return;
		}
	}
}

void map_for_each(void* that, map_t m, void (*each)(void* that, map_t map, key_t k)) {
	_map_t* map = (_map_t*)m;
	for (int i = 0; i < map->capacity; i++) {
		if (map->hashes[i] != EMPTY && map->hashes[i] != REMOVED) {
			assert(map->keys[i] != key_null);
			each(that, m, map->keys[i]);
		}
	}
}

boolean map_put_all(map_t destination, const map_t source) {
	_map_t* m = (_map_t*)source;
	for (int i = 0; i < m->capacity; i++) {
		if (m->hashes[i] != EMPTY && m->hashes[i] != REMOVED) {
			assert(m->keys[i] != key_null);
			assert(m->values[i] != value_null);
			if (!map_put(destination, m->keys[i], m->values[i])) {
				return false;
			}
		}
	}
	return true;
}

void map_clear(map_t map) {
	_map_t* m = (_map_t*)map;
	assert(m != null);
	if (m != null) {
		if (m->keys != null) {
			for (int i = 0; i < m->capacity; i++) {
				key_free(m->keys[i]); m->keys[i] = key_null;
				value_free(m->values[i]); m->values[i] = value_null;
				m->hashes[i] = EMPTY;
			}
		}
		m->size = 0;
	}
}

void map_dispose(map_t map) {
	_map_t* m = (_map_t*)map;
	if (m != null) {
		map_clear(map);
		if (m->allocated) { dispose(m); free(m); }
	}
}

#endif // map_t

