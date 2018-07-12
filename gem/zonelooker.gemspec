
FileUtils.cp '../c/zonelooker.c', 'ext/zonelooker_lib.c'
FileUtils.cp '../c/zonelooker.h', 'ext/zonelooker.h'
FileUtils.cp '../z_gen_tables.h', 'ext/table.h'

Gem::Specification.new do |spec|
  spec.name          = "zonelooker"
  spec.version       = '0.0.1'
  spec.authors       = ["Jesús A. Álvarez"]
  spec.email         = ["jesus@instabridge.com"]
  spec.summary       = "Look up latitude/longitude to country"
  spec.homepage      = "https://github.com/Instabridge/latlong"
  spec.license       = "Apache-2.0"
  spec.required_ruby_version = ">= 2.0"
  spec.files         = %w(extconf.rb table.h zonelooker_lib.c zonelooker.c zonelooker.h).map{ |f| "ext/#{f}" }
  spec.add_development_dependency "bundler", "~> 1.5"
  spec.extensions    = %w[ext/extconf.rb]
end