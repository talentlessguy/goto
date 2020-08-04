push   %ebp
mov    %esp, %ebp
sub    $16, %esp
mov    $1, -4(%ebp)
mov    $2, -8(%ebp)
mov    -4(%ebp), %edx
mov    -8(%ebp), %eax
add    %edx, %eax
mov    %eax, -12(%ebp)
mov    $0, %eax
leave
ret