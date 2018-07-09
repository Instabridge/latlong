require 'fiddle'
require 'fiddle/import'

module ZoneLooker
  extend Fiddle::Importer
  module_exec do
    os = RbConfig::CONFIG['host_os']
    if os.include?('darwin')
      dlload 'libzonelooker.dylib'
    elsif os.include?('linux') || os.include?('bsd')
      dlload 'libzonelooker.so'
    elsif os.include?('win')
      dlload 'libzonelooker.dll'
    else
      raise "Unsupported platform"
    end
  end
  extern 'const struct zl_table * zl_get_table(const char *name)'
  extern 'const char *zl_lookup(const struct zl_table * table, float lat, float lon)'
  extern 'int zl_deg_pixels(const struct zl_table * table)'
  
  class Table
    attr_reader :name, :resolution
    
    def initialize(name)
      @name = name
      @table = ZoneLooker::zl_get_table(name)
      @resolution = ZoneLooker::zl_deg_pixels(@table)
    end
    
    def lookup(lat, lon)
      ptr = ZoneLooker::zl_lookup(@table, lat, lon)
      ptr.null? ? nil : ptr.to_str(2)
    end
  end
end
