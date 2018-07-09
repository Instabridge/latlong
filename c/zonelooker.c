#include <stdint.h>
#include <stdlib.h>

struct zl_tile {
	uint32_t key;
	uint16_t idx;
};

struct zl_zoom_level {
	size_t count;
	struct zl_tile tiles[];
};

struct zl_leaf {
	char type; // S static, 2 for bitmap, P for pixmap
	union {
		const char *pixmap; // 128 bytes of data
		const char *name; // zone name
		struct {
			uint16_t idx[2];
			uint64_t bits;
		} bitmap;
	} data;
};

struct zl_table {
	int deg_pixels;
	struct zl_zoom_level const * zoom_levels[6];
	struct zl_leaf leaves[];
};

#define table _zl_countries_table
#include "../z_gen_tables.h"
#undef table

const char *zoneLokupPixel(const struct zl_table *table, int x, int y);
const char *zoomLevelLookupTile(const struct zl_table *table, int level, int x, int y, uint32_t tile);
const char *leafLookupZone(const struct zl_table *table, uint16_t leafIndex, int x, int y, uint32_t tk);

const char *zl_lookup(const struct zl_table *table, float lat, float lon) {
	double degPixels = table->deg_pixels;
	int x = (lon + 180) * degPixels;
	int y = (90 - lat) * degPixels;
	if (x < 0) {
		x = 0;
	} else if (x >= 360*degPixels) {
		x = 360*degPixels - 1;
	}
	if (y < 0) {
		y = 0;
	} else if (y >= 180*degPixels) {
		y = 180*degPixels - 1;
	}
	return zoneLokupPixel(table, x, y);
}

#define TILEKEY(size, x, y) ((uint32_t) ( (size&7)<<28 | (y&((1<<14)-1))<<14 | (x&((1<<14)-1)) ))

int zl_deg_pixels(const struct zl_table *table) {
	return table->deg_pixels;
}

const struct zl_table * zl_get_table(const char *name) {
	// TODO: support different tables
	return &_zl_countries_table;
}

const char *zoneLokupPixel(const struct zl_table *table, int x, int y) {
	for(int level = 5; level >= 0; level--) {
		uint8_t shift = 3 + level;
		uint16_t xt = x >> shift;
		uint16_t yt = y >> shift;
		uint32_t tk = TILEKEY(level, xt, yt);
		const char *zone = zoomLevelLookupTile(table, level, x, y, tk);
		if (zone) {
			return zone;
		}
	}
	return NULL;
}

const struct zl_tile * searchTileKey (const struct zl_zoom_level *zl, uint32_t key)
{
	size_t i = 0;
	size_t j = zl->count;
	while (i < j) {
		size_t h = (i + j) >> 1;
		uint32_t tileKey = zl->tiles[h].key;
		if (tileKey == key) {
			return &zl->tiles[h];
		} else if (tileKey > key) {
			j = h;
		} else {
			i = h + 1;
		}
	}
	return NULL;
}

const char *zoomLevelLookupTile(const struct zl_table *table, int level, int x, int y, uint32_t tk) {
	uint32_t key = tk;
	
	const struct zl_zoom_level *zl = table->zoom_levels[level];
	const struct zl_tile *tile = searchTileKey(zl, tk);
	if (tile == NULL) return NULL;
	return leafLookupZone(table, tile->idx, x, y, tk);
}

#define OCEAN_INDEX 0xFFFF

const char *leafLookupStatic(const struct zl_leaf *leaf) {
	return leaf->data.name;
}

const char *leafLookupBitmap(const struct zl_table *table, const struct zl_leaf *leaf, int x, int y, uint32_t tk) {
	uint64_t bit = 1ULL << ((8 * (y&7)) + (x&7));
	uint16_t idx = leaf->data.bitmap.idx[(leaf->data.bitmap.bits & bit) ? 1 : 0];
	return leafLookupZone(table, idx, x, y, tk);
}

const char *leafLookupPixmap(const struct zl_table *table, const struct zl_leaf *leaf, int x, int y, uint32_t tk) {
	size_t i = 2 * ((y&7)*8 + (x&7));
	const uint8_t *p = (const uint8_t *)leaf->data.pixmap;
	uint16_t idx = (p[i] << 8) + p[i+1];
	if (idx == OCEAN_INDEX) {
		return NULL;
	} else {
		return leafLookupZone(table, idx, x, y, tk);
	}
}

const char *leafLookupZone(const struct zl_table *table, uint16_t leafIndex, int x, int y, uint32_t tk) {
	const struct zl_leaf *leaf = &table->leaves[leafIndex];
	if (leaf->type == 'S') {
		// static
		return leafLookupStatic(leaf);
	} else if (leaf->type == '2') {
		// bitmap
		return leafLookupBitmap(table, leaf, x, y, tk);
	} else if (leaf->type == 'P') {
		// pixmap
		return leafLookupPixmap(table, leaf, x, y, tk);
	}
	return NULL;
}
