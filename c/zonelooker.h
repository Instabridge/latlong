const struct zl_table * zl_get_table(const char *name);
const char *zl_lookup(const struct zl_table * table, float lat, float lon);
int zl_deg_pixels(const struct zl_table * table);
