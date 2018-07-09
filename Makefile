.PHONY: z_gen_tables.go
z_gen_tables.go: gen_test.go latlong.go world/tz_world.shp
	go test --tags=latlong_gen --generate -v

world/tz_world.shp: tz_world.zip
	unzip -f tz_world.zip

tz_world.zip:
	wget http://efele.net/maps/tz/world/tz_world.zip
