Pod::Spec.new do |s|
  s.name = 'M5TypeSafety'
  s.version = '1.2.0'
  s.license = { :type => 'MIT', :file => 'LICENSE' }
  s.summary = 'Obj-C macros/functions (mostly for debugging) for applying runtime type safety.'
  s.homepage = 'https://github.com/mhuusko5/M5TypeSafety'
  s.social_media_url = 'https://twitter.com/mhuusko5'
  s.authors = { 'Mathew Huusko V' => 'mhuusko5@gmail.com' }
  s.source = { :git => 'https://github.com/mhuusko5/M5TypeSafety.git', :tag => s.version.to_s }

  s.source_files = '*.{h,m}'
  s.requires_arc = true

  s.ios.deployment_target = '7.0'
  s.osx.deployment_target = '10.8'

  s.frameworks = 'Foundation'
end
