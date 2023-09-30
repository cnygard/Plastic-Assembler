nop
test 1 eax 0 ebx 0 edx 0 eir nop alu 1 esp -1 flag 0 &
reset
push 3
test 2 eax 0 ebx 0 edx 0 eir push alu 3 esp 3 sv 3 flag 0 &
reset
mov eax 3
push eax
test 3 eax 3 ebx 0 edx 0 eir push alu 3 esp 3 sv 3 flag 0 &
reset
push 3
pop eax
test 4 eax 3 ebx 0 edx 0 eir pop alu 3 esp -1 flag 0 &
reset
mov eax 3
test 5 eax 3 ebx 0 edx 0 eir mov alu 3 esp -1 flag 0 &
reset
mov eax 3
mov ebx eax
test 6 eax 3 ebx 3 edx 0 eir mov alu 3 esp -1 flag 0 &
reset
push 3
push 5
add
test 7 eax 0 ebx 0 edx 0 eir add alu 8 esp 3 sv 8 flag 0 &
reset
push 3
mov eax 5
add eax
test 8 eax 8 ebx 0 edx 0 eir add alu 8 esp -1 flag 0 &
reset
mov eax 3
add eax 5
test 9 eax 8 ebx 0 edx 0 eir add alu 8 esp -1 flag 0 &
reset
mov eax 3
mov ebx 5
add eax ebx
test 10 eax 8 ebx 5 edx 0 eir add alu 8 esp -1 flag 0 &
reset
push 5
push 3
sub
test 11 eax 0 ebx 0 edx 0 eir sub alu 2 esp 3 sv 2 flag 0 &
reset
mov eax 5
push 3
sub eax
test 12 eax 2 ebx 0 edx 0 eir sub alu 2 esp -1 flag 0 &
reset
mov eax 5
sub eax 3
test 13 eax 2 ebx 0 edx 0 eir sub alu 2 esp -1 flag 0 &
reset
mov eax 5
mov ebx 3
sub eax ebx
test 14 eax 2 ebx 3 edx 0 eir sub alu 2 esp -1 flag 0 &
reset
push 3
push 5
mul
test 15 eax 0 ebx 0 edx 0 eir mul alu 15 esp 3 sv 15 flag 0 &
reset
mov eax 3
push 5
mul eax
test 16 eax 15 ebx 0 edx 0 eir mul alu 15 esp -1 flag 0 &
reset
mov eax 3
mul eax 5
test 17 eax 15 ebx 0 edx 0 eir mul alu 15 esp -1 flag 0 &
reset
mov eax 3
mov ebx 5
mul eax ebx
test 18 eax 15 ebx 5 edx 0 eir mul alu 15 esp -1 flag 0 &
reset
push 2
mov eax 11
div
test 19 eax 5 ebx 0 edx 1 eir div alu 5 esp -1 flag 0 &
reset
mov eax 11
div 2
test 20 eax 5 ebx 0 edx 1 eir div alu 5 esp -1 flag 0 &
reset
mov eax 11
mov ebx 2
div ebx
test 21 eax 5 ebx 2 edx 1 eir div alu 5 esp -1 flag 0 &
reset
mov eax 1
inc eax
test 22 eax 2 ebx 0 edx 0 eir inc alu 2 esp -1 flag 0 &
reset
mov eax 1
dec eax
test 23 eax 0 ebx 0 edx 0 eir dec alu 0 esp -1 flag 8 &
reset
push 0x6b2
jmp
mov eax 1
test 24 eax 0 ebx 0 edx 0 eir jmp alu 1714 esp -1 flag 0 &
reset
jmp 0x6fa
mov eax 1
test 25 eax 0 ebx 0 edx 0 eir jmp alu 1786 esp -1 flag 0 &
reset
mov eax 0x748
jmp eax
mov eax 1
test 26 eax 1864 ebx 0 edx 0 eir jmp alu 1864 esp -1 flag 0 &
reset
push 0x79b
mov eax 0
jz
mov eax 1
test 27 eax 0 ebx 0 edx 0 eir jz alu 1947 esp -1 flag 0 &
reset
mov eax 0
jz 0x7ec
mov eax 1
test 28 eax 0 ebx 0 edx 0 eir jz alu 2028 esp -1 flag 0 &
reset
mov ebx 0x843
mov eax 0
jz ebx
mov eax 1
test 29 eax 0 ebx 2115 edx 0 eir jz alu 2115 esp -1 flag 0 &
reset
push 0x88d
jnz
mov eax 1
test 30 eax 0 ebx 0 edx 0 eir jnz alu 2189 esp -1 flag 0 &
reset
mov eax 1
jnz 0x8de
mov eax 0
test 31 eax 1 ebx 0 edx 0 eir jnz alu 2270 esp -1 flag 0 &
reset
mov eax 0x92c
jnz eax
mov eax 1
test 32 eax 2348 ebx 0 edx 0 eir jnz alu 2348 esp -1 flag 0 &
reset
push 9
jf
mov eax 1
test 33 eax 0 ebx 0 edx 0 eir jf alu 2422 esp -1 flag 0 &
reset
jf 9
mov eax 1
test 34 eax 0 ebx 0 edx 0 eir jf alu 2494 esp -1 flag 0 &
reset
mov eax 9
jf eax
mov eax 1
test 35 eax 9 ebx 0 edx 0 eir jf alu 2572 esp -1 flag 0 &
reset
push 9
mov eax 0
jfz
mov eax 1
test 36 eax 0 ebx 0 edx 0 eir jfz alu 2655 esp -1 flag 0 &
reset
mov eax 0
jfz 9
mov eax 1
test 36 eax 0 ebx 0 edx 0 eir jfz alu 2736 esp -1 flag 0 &
reset
mov eax 9
mov ebx 0
jfz eax
mov eax 1
test 37 eax 9 ebx 0 edx 0 eir jfz alu 2823 esp -1 flag 0 &
reset
push 9
jfnz
mov eax 1
test 38 eax 0 ebx 0 edx 0 eir jfnz alu 2897 esp -1 flag 0 &
reset
mov eax 1
jfnz 9
mov eax 0
test 39 eax 1 ebx 0 edx 0 eir jfnz alu 2978 esp -1 flag 0 &
reset
mov eax 9
jfnz eax
mov eax 1
test 40 eax 9 ebx 0 edx 0 eir jfnz alu 3056 esp -1 flag 0 &
reset
push 23
mov eax 2
jf 12
test 41 eir jb &
dec eax
jfz 2
jb
test 42 eax 0 ebx 0 edx 0 eir jfz alu 3160 esp -1 flag 0 &
reset
mov eax 2
jf 12
test 43 eir jb &
dec eax
jfz 7
jb 30
test 44 eax 0 ebx 0 edx 0 eir jfz alu 3262 esp -1 flag 0 &
reset
mov ebx 23
mov eax 2
jf 12
test 45 eir jb &
dec eax
jfz 4
jb ebx
test 46 eax 0 ebx 23 edx 0 eir jfz alu 3370 esp -1 flag 0 &
reset
push 16
mov eax 1
jf 12
test 47 eir jbz &
dec eax
jbz
test 48 eax -1 ebx 0 edx 0 eir jbz alu -1 esp -1 flag 16 &
reset
mov eax 1
jf 12
test 49 eir jbz &
dec eax
jbz 16
test 50 eax -1 ebx 0 edx 0 eir jbz alu -1 esp -1 flag 16 &
reset
mov ebx 16
mov eax 1
jf 12
test 51 eir jbz &
dec eax
jbz ebx
test 52 eax -1 ebx 16 edx 0 eir jbz alu -1 esp -1 flag 16 &
reset
push 16
mov eax 2
jf 12
test 53 eir jbnz &
dec eax
jbnz
test 54 eax 0 ebx 0 edx 0 eir jbnz alu 0 esp -1 flag 8 &
reset
mov eax 2
jf 12
test 55 eir jbnz &
dec eax
jbnz 16
test 56 eax 0 ebx 0 edx 0 eir jbnz alu 0 esp -1 flag 8 &
reset
mov ebx 16
mov eax 2
jf 12
test 57 eir jbnz &
dec eax
jbnz ebx
test 58 eax 0 ebx 16 edx 0 eir jbnz alu 0 esp -1 flag 8 &
reset
push 3
push 3
eq
test 59 eax 0 ebx 0 edx 0 eir eq alu 0 esp -1 flag 8 &
reset
push 3
push 5
eq
test 60 eax 0 ebx 0 edx 0 eir eq alu 1 esp -1 flag 0 &
reset
mov eax 3
push 3
eq eax
test 61 eax 3 ebx 0 edx 0 eir eq alu 0 esp -1 flag 8 &
reset
mov eax 3
push 5
eq eax
test 62 eax 3 ebx 0 edx 0 eir eq alu 1 esp -1 flag 0 &
reset
push 3
eq 3
test 63 eax 0 ebx 0 edx 0 eir eq alu 0 esp -1 flag 8 &
reset
push 3
eq 5
test 64 eax 0 ebx 0 edx 0 eir eq alu 1 esp -1 flag 0 &
reset
mov eax 3
eq eax 3
test 65 eax 3 ebx 0 edx 0 eir eq alu 0 esp -1 flag 8 &
reset
mov eax 3
eq eax 5
test 66 eax 3 ebx 0 edx 0 eir eq alu 1 esp -1 flag 0 &
reset
mov eax 3
mov ebx 3
eq eax ebx
test 67 eax 3 ebx 3 edx 0 eir eq alu 0 esp -1 flag 8 &
reset
mov eax 3
mov ebx 5
eq eax ebx
test 68 eax 3 ebx 5 edx 0 eir eq alu 1 esp -1 flag 0 &
reset
push 3
push 5
neq
test 69 eax 0 ebx 0 edx 0 eir neq alu 0 esp -1 flag 8 &
reset
push 3
push 3
neq
test 70 eax 0 ebx 0 edx 0 eir neq alu 1 esp -1 flag 0 &
reset
mov eax 3
push 5
neq eax
test 71 eax 3 ebx 0 edx 0 eir neq alu 0 esp -1 flag 8 &
reset
mov eax 3
push 3
neq eax
test 72 eax 3 ebx 0 edx 0 eir neq alu 1 esp -1 flag 0 &
reset
push 3
neq 5
test 73 eax 0 ebx 0 edx 0 eir neq alu 0 esp -1 flag 8 &
reset
push 3
neq 3
test 74 eax 0 ebx 0 edx 0 eir neq alu 1 esp -1 flag 0 &
reset
mov eax 3
neq eax 5
test 75 eax 3 ebx 0 edx 0 eir neq alu 0 esp -1 flag 8 &
reset
mov eax 3
neq eax 3
test 76 eax 3 ebx 0 edx 0 eir neq alu 1 esp -1 flag 0 &
reset
mov eax 3
mov ebx 5
neq eax ebx
test 77 eax 3 ebx 5 edx 0 eir neq alu 0 esp -1 flag 8 &
reset
mov eax 3
mov ebx 3
neq eax ebx
test 78 eax 3 ebx 3 edx 0 eir neq alu 1 esp -1 flag 0 &
reset
push 3
push 5
gt
test 79 eax 0 ebx 0 edx 0 eir gt alu 0 esp -1 flag 8 &
reset
push 3
push 3
gt
test 80 eax 0 ebx 0 edx 0 eir gt alu 1 esp -1 flag 0 &
reset
mov eax 5
push 3
gt eax
test 81 eax 5 ebx 0 edx 0 eir gt alu 0 esp -1 flag 8 &
reset
mov eax 3
push 3
gt eax
test 82 eax 3 ebx 0 edx 0 eir gt alu 1 esp -1 flag 0 &
reset
push 3
gt 5
test 83 eax 0 ebx 0 edx 0 eir gt alu 0 esp -1 flag 8 &
reset
push 3
gt 3
test 84 eax 0 ebx 0 edx 0 eir gt alu 1 esp -1 flag 0 &
reset
mov eax 5
gt eax 3
test 85 eax 5 ebx 0 edx 0 eir gt alu 0 esp -1 flag 8 &
reset
mov eax 3
gt eax 3
test 86 eax 3 ebx 0 edx 0 eir gt alu 1 esp -1 flag 0 &
reset
mov eax 5
mov ebx 3
gt eax ebx
test 87 eax 5 ebx 3 edx 0 eir gt alu 0 esp -1 flag 8 &
reset
mov eax 3
mov ebx 3
gt eax ebx
test 88 eax 3 ebx 3 edx 0 eir gt alu 1 esp -1 flag 0 &
reset
push 5
push 3
lt
test 89 eax 0 ebx 0 edx 0 eir lt alu 0 esp -1 flag 8 &
reset
push 3
push 3
lt
test 90 eax 0 ebx 0 edx 0 eir lt alu 1 esp -1 flag 0 &
reset
mov eax 3
push 5
lt eax
test 91 eax 3 ebx 0 edx 0 eir lt alu 0 esp -1 flag 8 &
reset
mov eax 3
push 3
lt eax
test 92 eax 3 ebx 0 edx 0 eir lt alu 1 esp -1 flag 0 &
reset
push 5
lt 3
test 93 eax 0 ebx 0 edx 0 eir lt alu 0 esp -1 flag 8 &
reset
push 3
lt 3
test 94 eax 0 ebx 0 edx 0 eir lt alu 1 esp -1 flag 0 &
reset
mov eax 3
lt eax 5
test 95 eax 3 ebx 0 edx 0 eir lt alu 0 esp -1 flag 8 &
reset
mov eax 3
lt eax 3
test 96 eax 3 ebx 0 edx 0 eir lt alu 1 esp -1 flag 0 &
reset
mov eax 3
mov ebx 5
lt eax ebx
test 97 eax 3 ebx 5 edx 0 eir lt alu 0 esp -1 flag 8 &
reset
mov eax 3
mov ebx 3
lt eax ebx
test 98 eax 3 ebx 3 edx 0 eir lt alu 1 esp -1 flag 0 &
reset
push 3
push 5
gtq
test 99 eax 0 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
push 3
push 3
gtq
test 100 eax 0 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
push 5
push 3
gtq
test 101 eax 0 ebx 0 edx 0 eir gtq alu 1 esp -1 flag 0 &
reset
mov eax 5
push 3
gtq eax
test 102 eax 5 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
mov eax 3
push 3
gtq eax
test 103 eax 3 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
mov eax 3
push 5
gtq eax
test 104 eax 3 ebx 0 edx 0 eir gtq alu 1 esp -1 flag 0 &
reset
push 3
gtq 5
test 105 eax 0 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
push 3
gtq 3
test 106 eax 0 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
push 5
gtq 3
test 107 eax 0 ebx 0 edx 0 eir gtq alu 1 esp -1 flag 0 &
reset
mov eax 5
gtq eax 3
test 108 eax 5 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
mov eax 3
gtq eax 3
test 109 eax 3 ebx 0 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
mov eax 3
gtq eax 5
test 110 eax 3 ebx 0 edx 0 eir gtq alu 1 esp -1 flag 0 &
reset
mov eax 5
mov ebx 3
gtq eax ebx
test 111 eax 5 ebx 3 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
mov eax 3
mov ebx 3
gtq eax ebx
test 112 eax 3 ebx 3 edx 0 eir gtq alu 0 esp -1 flag 8 &
reset
mov eax 3
mov ebx 5
gtq eax ebx
test 113 eax 3 ebx 5 edx 0 eir gtq alu 1 esp -1 flag 0 &
reset
push 3
push 3
ltq
test 114 eax 0 ebx 0 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
push 3
push 5
ltq
test 115 eax 0 ebx 0 edx 0 eir ltq alu 1 esp -1 flag 0 &
reset
mov eax 3
push 5
ltq eax
test 116 eax 3 ebx 0 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
mov eax 3
push 3
ltq eax
test 117 eax 3 ebx 0 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
mov eax 5
push 3
ltq eax
test 118 eax 5 ebx 0 edx 0 eir ltq alu 1 esp -1 flag 0 &
reset
push 5
ltq 3
test 119 eax 0 ebx 0 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
push 3
ltq 3
test 120 eax 0 ebx 0 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
push 3
ltq 5
test 121 eax 0 ebx 0 edx 0 eir ltq alu 1 esp -1 flag 0 &
reset
mov eax 3
ltq eax 5
test 122 eax 3 ebx 0 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
mov eax 3
ltq eax 3
test 123 eax 3 ebx 0 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
mov eax 5
ltq eax 3
test 124 eax 5 ebx 0 edx 0 eir ltq alu 1 esp -1 flag 0 &
reset
mov eax 3
mov ebx 5
ltq eax ebx
test 125 eax 3 ebx 5 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
mov eax 3
mov ebx 3
ltq eax ebx
test 126 eax 3 ebx 3 edx 0 eir ltq alu 0 esp -1 flag 8 &
reset
mov eax 5
mov ebx 3
ltq eax ebx
test 127 eax 5 ebx 3 edx 0 eir ltq alu 1 esp -1 flag 0 &
reset
test 128 eax 0 ebx 0 edx 0 eir reset alu 1 esp -1 flag 0 &
hlt
test 129 eax 12 ebx 0 edx 0 eir hlt alu 1 esp -1 flag 0 &