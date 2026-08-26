from C.charges import *
from C.conditions import *

def generic_rwd(found_charges, curr_charges, charges_sum,
                quadratic_coef, cubic_coef, yukawa_coef):
    """
        Bad first reward function to make sure the
        training loop works.
    """
    r = 0
    sorted_charges = get_sorted_charges(curr_charges)
    for found in found_charges:
        if multiple_check(found, sorted_charges):
            return 300, False

    if not quadratic_coef:
        r += 100
    if not cubic_coef:
        r += 100
    if not yukawa_coef:
        r += 100

    if r < 300:
        return r, False
    
    found_charges.append(sorted_charges)
    return r+200, True