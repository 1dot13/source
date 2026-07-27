# Warning suppressions for our own code only.
#
# Included from the root CMakeLists.txt *after* the ext/ subdirectories, so
# add_compile_options()' directory scope leaves vendored code untouched.
#
# /WX is on, so anything not listed here breaks the build. One line per
# known-dirty warning, counted over all four applications at RelWithDebInfo
# (the #ifdef forest means each application is its own harvest).
# Workflow: delete a line, fix the fallout in both compilers, commit. Repeat
# until both lists are empty.
#
# The lists are not comparable by length: clang-cl /W3 is clang's own -Wall,
# which is already broader than MSVC /W4.

# clang-cl also sets MSVC, so it has to be tested first.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")   # clang-cl
  # Pinned so the baseline doesn't drift with the clang release.
  add_compile_options(/W3 /WX
    -Wno-writable-strings                          # 292823 string literal to char*
    -Wno-missing-braces                            #  11566 missing braces around subobject init
    -Wno-nontrivial-memcall                        #   7280 mem* on a non-trivial type
    -Wno-invalid-offsetof                          #   5428 offsetof on a non-standard-layout type
    -Wno-microsoft-cast                            #   4096 MSVC-only static_cast between unrelated types
    -Wno-microsoft-extra-qualification             #   3865 extra qualification on member declaration
    -Wno-mismatched-tags                           #   3620 struct/class tag mismatch
    -Wno-unused-variable                           #   1824 unused local
    -Wno-comment                                   #   1796 '//' or stray '/*' inside a block comment
    -Wno-unused-but-set-variable                   #   1574 assigned but never read
    -Wno-logical-op-parentheses                    #   1146 && inside || without parentheses
    -Wno-parentheses-equality                      #    250 redundant parentheses around ==
    -Wno-unused-function                           #    172 unused static function
    -Wno-switch                                    #    156 enum value not handled in switch
    -Wno-tautological-constant-out-of-range-compare#    146 compare always true/false for the type
    -Wno-unused-value                              #    142 expression result unused
    -Wno-format-security                           #    100 non-literal format string
    -Wno-deprecated-declarations                   #    100 use of a deprecated declaration
    -Wno-misleading-indentation                    #     90 indentation does not match nesting
    -Wno-parentheses                               #     84 suggested parentheses
    -Wno-nonportable-include-path                  #     80 #include case/separator does not match on disk
    -Wno-multichar                                 #     72 multi-character character constant
    -Wno-tautological-pointer-compare              #     70 address compared against null is never null
    -Wno-reorder-ctor                              #     60 member initialisers out of declaration order
    -Wno-extern-initializer                        #     58 'extern' variable has an initialiser
    -Wno-sometimes-uninitialized                   #     55 used uninitialised on some path
    -Wno-delete-abstract-non-virtual-dtor          #     48 delete of abstract class with non-virtual dtor
    -Wno-dangling-else                             #     36 else binds to the inner if
    -Wno-unused-lambda-capture                     #     32 lambda capture never used
    -Wno-self-assign                               #     30 variable assigned to itself
    -Wno-tautological-overlap-compare              #     28 overlapping comparisons always true/false
    -Wno-logical-not-parentheses                   #     28 ! applied to the left of a comparison
    -Wno-undefined-bool-conversion                 #     24 object address converted to bool is never false
    -Wno-mismatched-new-delete                     #     24 new/delete form mismatch
    -Wno-unused-private-field                      #     20 private field never used
    -Wno-pointer-bool-conversion                   #     14 pointer converted to bool is never false
    -Wno-tautological-undefined-compare            #     12 comparison of an address that cannot be null
    -Wno-delete-non-abstract-non-virtual-dtor      #     12 delete through base with non-virtual dtor
    -Wno-constant-conversion                       #     12 constant does not fit the target type
    -Wno-varargs                                   #      8 questionable argument to a variadic function
    -Wno-reserved-user-defined-literal             #      8 string literal followed by a reserved suffix
    -Wno-pointer-to-int-cast                       #      8 pointer cast to a smaller integer
    -Wno-invalid-source-encoding                   #      8 source byte is not valid UTF-8 (windows-1252 files)
    -Wno-invalid-pp-token                          #      8 malformed preprocessing token
    -Wno-implicit-conversion-floating-point-to-bool#      8 float implicitly converted to bool
    -Wno-enum-compare                              #      8 comparison between two different enum types
    -Wno-bitwise-op-parentheses                    #      8 & and | mixed without parentheses
    -Wno-shift-negative-value                      #      4 left shift of a negative value
    -Wno-return-type                               #      4 non-void function without a return
    -Wno-missing-exception-spec                    #      4 exception specification does not match
    -Wno-missing-declarations                      #      4 declaration does not declare anything
    -Wno-int-to-pointer-cast                       #      4 smaller integer cast to a pointer
    -Wno-ignored-pragmas                           #      4 unrecognised pragma ignored
    -Wno-extra-tokens                              #      4 extra tokens at the end of a directive
    -Wno-delete-incomplete                         #      4 delete of a pointer to an incomplete type
    -Wno-array-bounds                              #      4 array index out of bounds
    -Wno-header-guard                              #      2 header guard does not match the #define (Ja2Export)
  )
elseif(MSVC)
  # CMake stopped injecting /W3 of its own (policy CMP0092), which left us on
  # the /W1 default, so pick a level deliberately.
  add_compile_options(/W4 /WX
    /wd4244  # 35075 conversion, possible loss of data
    /wd4201  # 32106 nonstandard extension: nameless struct/union
    /wd4099  #  8788 type first seen as 'struct' now seen as 'class'
    /wd4100  #  5107 unreferenced formal parameter
    /wd4458  #  2552 declaration hides class member
    /wd4189  #  1561 local variable initialised but not referenced
    /wd4245  #   686 conversion, signed/unsigned mismatch
    /wd4018  #   640 '<': signed/unsigned mismatch
    /wd4456  #   590 declaration hides previous local declaration
    /wd4701  #   417 potentially uninitialised local variable used
    /wd4389  #   368 '==': signed/unsigned mismatch
    /wd4267  #   328 conversion from size_t, possible loss of data
    /wd4702  #   268 unreachable code
    /wd4459  #   162 declaration hides global declaration
    /wd4505  #   160 unreferenced function with internal linkage removed
    /wd4996  #   116 deprecated CRT function
    /wd4703  #    97 potentially uninitialised local pointer used
    /wd4211  #    66 nonstandard extension: redefined extern to static
    /wd4706  #    64 assignment used as a condition
    /wd4476  #    56 unknown type field character in format specifier
    /wd4101  #    54 unreferenced local variable
    /wd4146  #    24 unary minus applied to unsigned type
    /wd5205  #    16 delete of an abstract class with a non-virtual dtor
    /wd4457  #    16 declaration hides function parameter
    /wd4310  #    16 cast truncates constant value
    /wd4302  #     8 'type cast': pointer truncated to a smaller integer
    /wd5043  #     4 exception specification does not match previous declaration
    /wd4918  #     4 invalid character in pragma optimization list
    /wd4805  #     4 unsafe mix of BOOLEAN and bool
    /wd4305  #     4 truncation from 'double' to 'float'
    /wd4150  #     4 delete of pointer to incomplete type, no destructor called
    /wd4838  #     0 narrowing conversion; inherited, quiet at /W4 RelWithDebInfo
  )
endif()
