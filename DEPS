gclient_gn_args_from = 'src'

vars = {}

deps = {
  'src': 'https://github.com/LittleKu/base@07ba7f7ac381cff521003842195ea7f9519482a5',
}

hooks = [
  {
    'name': 'git patch',
    'pattern': 'src/libef',
    'cwd': 'src',
    'action': [
      'python3',
      'libef/tools/git_patch.py',
      'libef/patchs'
    ]
  }
]

recursedeps = [
  'src',
]
