
add_executable(test_xlat test_xlat.c)
add_executable(test_neg test_neg.c)
add_executable(test_cmpxchg_xadd_reg2reg test_cmpxchg_xadd_reg2reg.c)

add_test(neg $<TARGET_FILE:ptlsim> $<TARGET_FILE:test_neg>)
