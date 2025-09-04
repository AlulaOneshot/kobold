project('kobold-libk', 'c', version: '0.1.0', license: 'MPL-2.0-or-later')

target_arch = host_machine.cpu_family()

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
    'include/@0@/include'.format(target_arch),
])

src = {
  'x86_64': [
    'src/arch/x86_64/stdlib/abort.c',
    'src/arch/x86_64/string/memcmp.c',
    'src/arch/x86_64/string/memcpy.c',
    'src/arch/x86_64/string/memmove.c',
    'src/arch/x86_64/string/memset.c',
    'src/arch/x86_64/string/strlen.c',
  ],
  'aarch64': [
    
  ],
  'riscv64': [
    
  ]
}

sources =  src[target_arch]

linker_ld = meson.project_source_root() / 'linker/linker-@0@.ld'.format(target_arch)

libk = static_library('kernel',
  sources: sources,
  include_directories: includes,
  c_args: cflags + defines,
  link_args: ldflags + ['-T', linker_ld],
  install: false
)

libk_dep = declare_dependency(
  link_with: libk,
  include_directories: include_directories('include/@0@/include'.format(target_arch)),
)

meson.override_dependency('libk', libk_dep)