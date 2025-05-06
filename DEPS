gclient_gn_args_from = 'src'

vars = {}

deps = {
  'src': 'https://github.com/LittleKu/base@a5c8fdac2f1f0189d24e2d3920a182b8fdfc693a',
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
