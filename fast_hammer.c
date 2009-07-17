/*
 * Ruby module for winning EngineYard's programming contest.
 */

#include "ruby.h"


/*
 * Generated from:
     0.upto(255) do |i|
       print i.to_s(2).gsub(/0/, '').length, ", "
       puts if i % 16 == 15
     end
 */
static long bits_in_byte[256] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

/*
 * internal_fast_hammer -- calculate hamming distance of two numbers.
 */
long internal_fast_hammer (const char *a, const char *b, register long length)
{
  register int i;
  register long hammer = 0;
  for (i = 0; i < length; i++)
    hammer += bits_in_byte[a[i] ^ b[i]];
  return hammer;
}


/*
 * meth_FastHammer_fast_hammer -- Ruby front end to internal_fast_hammer.
 * Hard coding 20 as length for now, for this contest.
 */
VALUE meth_FastHammer_fast_hammer (VALUE self, VALUE a, VALUE b)
{
  return INT2FIX(internal_fast_hammer(StringValuePtr(a), StringValuePtr(b), 20));
}


/*
 *
 */
VALUE meth_FastHammer_low_hammer
    (VALUE self, VALUE match_hash, VALUE base_string, VALUE start_suffix,
     VALUE max_time)
{
  return T_NIL; /* not implemented */
}


void Init_fast_hammer (void)
{
  VALUE m_FastHammer = rb_define_module("FastHammer");
  rb_define_singleton_method(m_FastHammer, "fast_hammer",
                             meth_FastHammer_fast_hammer, 2);
  rb_define_singleton_method(m_FastHammer, "low_hammer",
                             meth_FastHammer_low_hammer, 4);
}
