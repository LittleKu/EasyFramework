gclient_gn_args_from = 'src'

vars = {}

deps = {
  'src': 'https://github.com/LittleKu/base@7d9daca9c2c24136f3e9b3c23c22f05dfee0d68f',
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
