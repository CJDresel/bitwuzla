#!/bin/sh
#--------------------------------------------------------------------------#

BUILDDIR=build

#--------------------------------------------------------------------------#

asan=no
ubsan=no
debug=no
check=no
shared=no
prefix=
path=

gcov=no
gprof=no

ninja=no

flags=""

#--------------------------------------------------------------------------#

usage () {
cat <<EOF
usage: $0 [<option> ...]

where <option> is one of the following:

  -h, --help        print this message and exit

  -g                compile with debugging support
  -f...|-m...       add compiler options

  --ninja           use Ninja build system
  --prefix <dir>    install prefix

  --path <dir>      look for dependencies in <dir>/{include,lib}
                    specify multiple --path options for multiple directories

  --shared          shared library

  -l                compile with logging support (default for '-g')
  -c                check assertions even in optimized compilation
  --asan            compile with -fsanitize=address -fsanitize-recover=address
  --ubsan           compile with -fsanitize=undefined
  --gcov            compile with -fprofile-arcs -ftest-coverage
  --gprof           compile with -pg
EOF
  exit 0
}

#--------------------------------------------------------------------------#

die () {
  echo "*** configure.sh: $*" 1>&2
  exit 1
}

msg () {
  echo "[configure.sh] $*"
}

#--------------------------------------------------------------------------#

[ ! -e src/bitvector.cpp ] && die "$0 not called from bzla-ls base directory"

while [ $# -gt 0 ]
do
  opt=$1
  case $opt in
    -h|--help) usage;;

    -g) debug=yes;;
    -f*|-m*) if [ -z "$flags" ]; then flags=$1; else flags="$flags;$1"; fi;;

    --ninja) ninja=yes;;

    --prefix)
      shift
      [ $# -eq 0 ] && die "missing argument to $opt"
      prefix=$1
      ;;

    --path)
      shift
      [ $# -eq 0 ] && die "missing argument to $opt"
      [ -n "$path" ] && path="$path;"
      path="$path$1"
      ;;

    --shared) shared=yes;;

    -c)      check=yes;;
    --asan)  asan=yes;;
    --ubsan) ubsan=yes;;
    --gcov)  gcov=yes;;
    --gprof) gprof=yes;;

    -*) die "invalid option '$opt' (try '-h')";;
  esac
  shift
done

#--------------------------------------------------------------------------#

cmake_opts="$CMAKE_OPTS"

[ $ninja = yes ] && cmake_opts="$cmake_opts -G Ninja"

[ $asan = yes ] && cmake_opts="$cmake_opts -DASAN=ON"
[ $ubsan = yes ] && cmake_opts="$cmake_opts -DUBSAN=ON"
[ $debug = yes ] && cmake_opts="$cmake_opts -DCMAKE_BUILD_TYPE=Debug"
[ $check = yes ] && cmake_opts="$cmake_opts -DCHECK=ON"
[ $shared = yes ] && cmake_opts="$cmake_opts -DBUILD_SHARED_LIBS=ON"

[ -n "$prefix" ] && cmake_opts="$cmake_opts -DCMAKE_INSTALL_PREFIX=$prefix"
[ -n "$path" ] && cmake_opts="$cmake_opts -DCMAKE_PREFIX_PATH=$path"

[ $gcov = yes ] && cmake_opts="$cmake_opts -DGCOV=ON"
[ $gprof = yes ] && cmake_opts="$cmake_opts -DGPROF=ON"

[ -n "$flags" ] && cmake_opts="$cmake_opts -DFLAGS=$flags"

mkdir -p $BUILDDIR
cd $BUILDDIR || exit 1

[ -e CMakeCache.txt ] && rm CMakeCache.txt
cmake .. $cmake_opts
