# Building MoarVM with Meson

This document describes how to build MoarVM using the Meson build system.

## Prerequisites

- Meson >= 1.0.0
- Ninja (or another Meson backend)
- C compiler (GCC, Clang, or MSVC)
- Perl 5.10+ (for code generation scripts)
- Raku/Perl 6 (optional, for op code regeneration)

### Installing Meson

```bash
# Using pip
pip install meson ninja

# On Debian/Ubuntu
apt install meson ninja-build

# On macOS
brew install meson ninja

# On Windows
pip install meson ninja
```

## Quick Start

```bash
# Setup build directory
meson setup builddir

# Compile
meson compile -C builddir

# Install (optional)
meson install -C builddir
```

## Build Options

### Basic Options

```bash
# Debug build
meson setup builddir --buildtype=debug

# Release build with optimizations
meson setup builddir --buildtype=release

# Custom optimization level
meson setup builddir --optimization=3

# Installation prefix
meson setup builddir --prefix=/usr/local
```

### MoarVM-Specific Options

```bash
# Enable JIT (default: auto, enabled on x86_64)
meson setup builddir -Djit=enabled
meson setup builddir -Djit=disabled

# Build static library instead of shared
meson setup builddir -Dstatic=true

# Use system libraries instead of bundled subprojects
meson setup builddir -Duse-system-libuv=true
meson setup builddir -Duse-system-libtommath=true

# Use libffi for native calls instead of dyncall
meson setup builddir -Duse-libffi=true

# Enable mimalloc (default: auto, enabled if C11 atomics available)
meson setup builddir -Dmimalloc=enabled
meson setup builddir -Dmimalloc=disabled

# C11 atomics vs libatomic_ops
meson setup builddir -Dc11-atomics=enabled
```

### Development Options

```bash
# Enable sanitizers
meson setup builddir -Dasan=true       # AddressSanitizer
meson setup builddir -Dubsan=true      # UndefinedBehaviorSanitizer
meson setup builddir -Dtsan=true       # ThreadSanitizer

# Enable coverage instrumentation
meson setup builddir -Dcoverage=true

# Show auto-vectorization reports (GCC/Clang)
meson setup builddir -Dshow-autovect=true
```

## Reconfiguring

You can change options after setup:

```bash
# Reconfigure existing build
meson configure builddir -Djit=disabled -Doptimization=2

# View current configuration
meson configure builddir
```

## Cross-Compilation

Meson has excellent cross-compilation support:

```bash
# Create a cross-file (example for ARM)
cat > arm-cross.txt << EOF
[binaries]
c = 'arm-linux-gnueabihf-gcc'
ar = 'arm-linux-gnueabihf-ar'
strip = 'arm-linux-gnueabihf-strip'

[host_machine]
system = 'linux'
cpu_family = 'arm'
cpu = 'armv7l'
endian = 'little'
EOF

# Use the cross-file
meson setup --cross-file arm-cross.txt builddir-arm
```

## IDE Integration

Meson can generate project files for various IDEs:

```bash
# Visual Studio
meson setup --backend=vs builddir-vs

# Xcode
meson setup --backend=xcode builddir-xcode

# For VS Code, just use the Meson extension
```

## Comparison with Configure.pl

| Configure.pl | Meson Equivalent |
|-------------|------------------|
| `perl Configure.pl` | `meson setup builddir` |
| `--prefix=/usr/local` | `--prefix=/usr/local` |
| `--optimize` | `--buildtype=release` |
| `--debug` | `--buildtype=debug` |
| `--asan` | `-Dasan=true` |
| `--has-libffi` | `-Duse-libffi=true` |
| `--no-jit` | `-Djit=disabled` |
| `make` | `meson compile -C builddir` |
| `make install` | `meson install -C builddir` |

## Features

### Automatic Dependency Resolution

Meson will automatically:
- Download and build bundled dependencies (libuv, dyncall, etc.) as needed
- Use system libraries when available via pkg-config
- Fall back to subprojects if system libraries are not found

### Parallel Builds

Meson/Ninja automatically uses all available CPU cores for parallel compilation.

### Incremental Builds

Only changed files are recompiled. Meson tracks dependencies accurately.

### Build Caching

Use `sccache` or `ccache` for faster rebuilds:

```bash
export CC="ccache gcc"
meson setup builddir
```

## Troubleshooting

### Perl scripts fail

Ensure Perl 5.10+ is installed and in your PATH:

```bash
perl --version
```

### Missing dependencies

Meson will report missing dependencies clearly. Install them via your package manager or allow Meson to build them as subprojects (default).

### Clean rebuild

```bash
rm -rf builddir
meson setup builddir
```

### Verbose build output

```bash
meson compile -C builddir -v
```

## Migration Notes

The Meson build system is designed to coexist with the existing Configure.pl build system during the transition period. Key differences:

1. **Out-of-tree builds**: Meson builds in a separate directory (builddir), keeping the source tree clean.

2. **Code generation**: Perl/Raku scripts are wrapped rather than ported (for now). Future commits will port them to Python.

3. **Dependency management**: Meson handles subprojects automatically via wrap files.

4. **Configuration**: Use `meson configure` instead of re-running Configure.pl.

## Contributing

When adding new source files, update `meson.build`:

```meson
core_sources += files('src/core/newfile.c')
```

When adding new dependencies, create a wrap file in `subprojects/` or update dependency declarations.

## See Also

- [Meson Documentation](https://mesonbuild.com/)
- [MoarVM Website](https://moarvm.org/)
- Original build system: `README.md` and `Configure.pl --help`
