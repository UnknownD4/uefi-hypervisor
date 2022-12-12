#include "include/msr.h"

uint64_t AdjustControls(uint64_t _msr, uint64_t control){
    MSR MSR_Result = {0};
    MSR_Result.All = __readmsr(_msr);
    control &= MSR_Result.Fields.High; // high bytes should be zero
    control |= MSR_Result.Fields.Low; // low bytes should be one
    return control;
}