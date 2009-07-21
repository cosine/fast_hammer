Gem::Specification.new do |s|
  s.name = 'fast_hammer'
  s.version = "0.1.5"
  s.summary = "Library to win @EngineYard's contest"
  s.description = "Provides a C-based algorithm for quickly plowing through SHA1s and\ncalculating hamming distances.\n"
  s.author = "Michael H. Buselli"
  s.email = "cosine@cosine.org"
  s.homepage = "http://cosine.org/"
  #s.files = Dir.glob('ext/**/*')
  s.files = ["ext/fast_hammer", "ext/fast_hammer/extconf.rb", "ext/fast_hammer/fast_hammer.c", "ext/fast_hammer/sha1.c", "ext/fast_hammer/sha1.h"]
  s.require_paths = ['ext']
  s.rubyforge_project = "fast_hammer"
  s.extensions = "ext/fast_hammer/extconf.rb"
  s.has_rdoc = false
end
