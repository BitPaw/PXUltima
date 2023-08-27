;.model flat, stdcall

;.code

; section	.text
;   global PXEndianSwapI32U        ;must be declared for using gcc



.code









; PXEndianSwapI16U PROC

    ; Remeberm, eax is 32-Bit, rax is x64    

    ; BSWAP requires a register to be accessed

;     mov     eax, [rsp + 3Ch]      ; Fetch parameter A

;     BSWAP   eax                   ; Swap byte order
   
;     MOV     [rsp + 3Ch], eax      ; Safe parameter A
 
;     RET                           ; Return from adress
; PXEndianSwapI16U ENDP


PXEndianSwapI32U PROC

    ; Remeberm, eax is 32-Bit, rax is x64    

    ; BSWAP requires a register to be accessed

    mov     eax, [rsp + 3Ch]      ; Fetch parameter A

    BSWAP   eax                   ; Swap byte order
   
    MOV     [rsp + 3Ch], eax      ; Safe parameter A

    RET                           ; Return from adress
PXEndianSwapI32U ENDP

END