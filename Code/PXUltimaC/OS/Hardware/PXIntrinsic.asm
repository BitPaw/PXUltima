.CODE

PXIntrinsicFSIN PROC
        fld dword ptr [rcx] ; Load input value into st(0)
        fsin                    ; Compute sine
        fstp dword ptr [rcx]     ; Store result in a local variable
        ret
PXIntrinsicFSIN ENDP

END