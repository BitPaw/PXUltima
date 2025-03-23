.CODE

PXIntrinsicFSIN PROC
       ; push placeholder
       ; mov bx, [ecx]
        fld qword ptr [rcx]           ; [Load Floating-Point Value]    Load input value into st(0)        
        fsin                    ; Compute sine
        fstp qword ptr [rcx]    ; [Store Floating-Point Value] Store result in a local variable
        mov rax, [rcx]
        ;pop placeholder
        ret
PXIntrinsicFSIN ENDP

PXIntrinsicFSQRT PROC
        fld dword ptr [rcx]           ; [Load Floating-Point Value]    Load input value into st(0)        
        fsqrt                   ; Compute square root
        fstp dword ptr [rcx]    ; [Store Floating-Point Value] Store result in a local variable
        ret
PXIntrinsicFSQRT ENDP


END