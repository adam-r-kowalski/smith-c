munit_dep = dependency('munit', fallback : ['munit', 'munit_dep'])

test_executable = executable(
  'test_smith',
  sources : [
    'src/test_main.c',
    'src/assertions.c',
    'src/random.c',
    'src/test_tokenizer.c',
    'src/test_hash_interner.c',
    '../src/tokenizer.c',
    '../src/system_allocator.c',
    '../src/null_allocator.c',
    '../src/finite_allocator.c',
    '../src/allocator.c',
    '../src/hash_interner.c',
    '../src/interner.c'
  ],
  dependencies : munit_dep,
  include_directories : [
    include_directories('include'),
    include_directories('../include'),
  ],
)

test('smith_test', test_executable)
