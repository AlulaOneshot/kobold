project('kobold', 'c', version: '0.1.0', license: 'MPL-2.0-or-later')

# Getting configs
supported_archs = ['x86_64', 'aarch64', 'riscv64']

cfg = meson.project_source_root() / 'res/limine.conf'

if not meson.is_cross_build()
    error(f'\'meson setup\' must be invoked with \'--cross-file=res/cross-files/<some file>.ini\'')
endif

target_arch = host_machine.cpu_family()

if target_arch not in supported_archs
    error('Unsupported architecture: ' + target_arch)
endif

libk_sp = subproject('libk')
libk_dep = libk_sp.get_variable('libk_dep')

message('Building for architecture: @0@'.format(target_arch))

arch_configs = {
    'x86_64': {
        'cflags': ['-m64', '-mcmodel=kernel', '-mno-red-zone', '-mno-mmx', '-mno-sse', '-mno-sse2'],
        'ldflags': ['-m64'],
        'defines': ['-D ARCH_X86_64=1', '-D ARCH_AARCH64=0', '-D ARCH_RISCV64=0',],
    },
    'aarch64': {
        'cflags': ['-mcpu=generic', '-mgeneral-regs-only'],
        'ldflags': [],
        'defines': ['-D ARCH_AARCH64=1', '-D ARCH_RISCV64=0', '-D ARCH_X86_64=0'],
    },
    'riscv64': {
        'cflags': ['-march=rv64imac', '-mabi=lp64', '-mcmodel=medany'],
        'ldflags': [],
        'defines': ['-D ARCH_RISCV64=1', '-D ARCH_X86_64=0', '-D ARCH_AARCH64=0'],
    }
}

config = arch_configs[target_arch]

common_cflags = [
  '-ffreestanding',
  '-fno-stack-protector',
  '-fno-pic',
  '-fno-pie',
  '-nostdlib',
  '-nostdinc',
  '-Wall',
  '-Wextra',
  '-Wno-unused-parameter',
]

cflags = common_cflags + config['cflags']

common_ldflags = [
  '-nostdlib',
  '-static',
  '-Wl,--build-id=none',
]

ldflags = common_ldflags + config['ldflags']

defines = config['defines'] + [
  '-D KERNEL_VERSION="@0@"'.format(meson.project_version()),
]

includes = include_directories([
  'res/include',
  'res/include/arch/@0@'.format(target_arch),
  ])

common_src = [
  'src/kmain.c'
]

arch_src = {
  'x86_64': [
    'src/arch/x86_64/boot.c',
    'src/arch/x86_64/utilities/khalt.c',
    'src/arch/x86_64/cpu/cpuid.c'
  ],
  'aarch64': [
    'src/arch/aarch64/boot.c',
    'src/arch/aarch64/utilities/khalt.c'
  ],
  'riscv64': [
    'src/arch/riscv64/boot.c',
    'src/arch/riscv64/utilities/khalt.c'
  ]
}

sources = common_src + arch_src[target_arch]

linker_ld = 'res/linker/linker-@0@.ld'.format(target_arch)



kobold = executable('kernel',
  sources: sources,
  include_directories: includes,
  c_args: cflags + defines,
  link_args: ldflags + ['-T', meson.current_source_dir() / linker_ld],
  install: false,
  dependencies: [libk_dep]
)


iso_img = custom_target('image.iso',
  output: 'image.iso',
  command: [
    meson.project_source_root() / 'scripts/make_image.py',
    '@OUTPUT@', target_arch, kobold, cfg
  ],
  build_by_default: true
)

