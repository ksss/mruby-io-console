MRuby::Gem::Specification.new('mruby-io-console') do |spec|
  spec.license = 'MIT'
  spec.author  = 'ksss <co000ri@gmail.com>'

  spec.add_dependency('mruby-io')
  spec.add_dependency('mruby-error', :core => 'mruby-error')
  spec.cc.include_paths << "#{spec.build.gem_clone_dir}/mruby-io/include"
end
