        mcopy 13/AInclude/M16.Locator

*
* The main program that installs the patch
*
* This assumes the SANE FPT is in RAM, so we can just patch it directly.
* This is always true under System 6.0.1+, since TS2/TS3 will patch SANE.
*
patcher start
SYSTEM_TOOL  equ $0000                  ;system tool flag for GetTSPtr
SANE_TOOLSET equ $0A                    ;toolset number of SANE
SANEFP816_OFFSET equ $09*4              ;offset for SANEFP816 in SANE FPT

        pha                             ;get SANE FPT
        pha
        pea   SYSTEM_TOOL
        pea   SANE_TOOLSET
        _GetTSPtr
        
        phd                             ;do the patch
        tsc
        tcd
        ldy   #SANEFP816_OFFSET+1
        lda   [3],y
        sta   >to_orig+2
        dey
        lda   [3],y
        inc   a
        sta   >to_orig+1
        lda   #patch-1
        sta   [3],y
        iny
        lda   #>patch-1
        sta   [3],y
        pld

        pla                             ;clean up stack and return
        pla
        rtl
        end

*
* The patch for SANEFP816
*
* This zeros out certain locations in the SANE direct page to ensure that
* FX2C and FX2L conversions behave properly.  Otherwise, they may generate
* incorrect results in some cases where the input value is a negative
* multiple of 65536.
*
* This zeroing out is only needed for FX2C or FX2L, but we just do it
* unconditionally, since that is simpler and should not create any problems.
* (Per TBR2 p. 18-6, these locations do not need to be preserved.)
*
* This code assumes SANE stores its direct page pointer in its WAP.
*
patch   start
        phd
        tcd
        stz   142                       ;zero 6 bytes on the SANE DP
        stz   144
        stz   146
        pld
to_orig entry
        jml   >000000                   ;to be patched
        end
