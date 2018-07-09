.PHONY: z_gen_tables.go
table=data/ne_10m_admin_0_map_units.shp
scale=32

z_gen_tables.go: gen_test.go latlong.go $(table)
	go test --tags=latlong_gen --generate -v --scale $(scale) --write_image

world/tz_world.shp: tz_world.zip
	unzip $<

tz_world.zip:
	wget http://efele.net/maps/tz/world/tz_world.zip

data/ne_10m_admin_0_map_units.shp: ne_10m_admin_0_map_units.zip
	mkdir -p data
	unzip $< -d data

ne_10m_admin_0_map_units.zip:
	wget https://www.naturalearthdata.com/http//www.naturalearthdata.com/download/10m/cultural/ne_10m_admin_0_map_units.zip
