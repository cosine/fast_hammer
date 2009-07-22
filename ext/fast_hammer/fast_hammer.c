/*
 * Ruby module for winning EngineYard's programming contest.
 */

#include "ruby.h"
#include "sha1.h"
#include "sha1.c"


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
static inline uint32_t internal_fast_hammer (const uint8_t *a, const uint8_t *b,
        register uint32_t length)
{
  register uint32_t i;
  register uint32_t hammer = 0;
  for (i = 0; i < length; i++)
    hammer += bits_in_byte[a[i] ^ b[i]];
  return hammer;
}


/*
 * meth_FastHammer_fast_hammer -- Ruby front end to internal_fast_hammer.
 */
VALUE meth_FastHammer_fast_hammer (VALUE self, VALUE a, VALUE b)
{
  uint32_t length = RSTRING_LEN(a);
  return LONG2FIX(internal_fast_hammer(
      (uint8_t *) RSTRING_PTR(a), (uint8_t *) RSTRING_PTR(b), length));
}


/*
 * multi_sha1_ripper -- recursive function to rip through the hashs.
 */
static void multi_sha1_ripper (const uint8_t *match_hash,
        const char *base_string, char *end_of_base_string,
        const SHA1_CTX *base_context,
        char *hammer_string, uint32_t *lowest_hammer, uint32_t depth)
{
  uint8_t iter_character;
  SHA1_CTX context, sub_context;
  uint8_t hash[20];
  uint32_t hammer;
  VALUE yield_hash;

  for (iter_character = 33; iter_character < 127; iter_character++) {
    context = *base_context;
    SHA1_Update(&context, &iter_character, 1);
    sub_context = context;
    SHA1_Finish(&context, hash);
    hammer = internal_fast_hammer(match_hash, hash, 20);

    if (hammer < *lowest_hammer) {
      /* TODO check for words you can't say on television. */
      *lowest_hammer = hammer;
      strncpy(hammer_string, base_string, end_of_base_string - base_string);
      hammer_string[end_of_base_string - base_string] = iter_character;
      hammer_string[end_of_base_string - base_string + 1] = 0;

      if (rb_block_given_p()) {
        yield_hash = rb_hash_new();
        rb_hash_aset(yield_hash, ID2SYM(rb_intern("best_string")),
                     rb_str_new2(hammer_string));
        rb_hash_aset(yield_hash, ID2SYM(rb_intern("hamming_distance")),
                     LONG2FIX(*lowest_hammer));
        rb_yield(yield_hash);
      }
    }

    if (depth > 1) {
      end_of_base_string[0] = iter_character;
      multi_sha1_ripper(match_hash, base_string, end_of_base_string + 1,
                        &sub_context, hammer_string, lowest_hammer, depth - 1);
    }
  }

}


/*
 * meth_FastHammer_multi_sha1 -- Ruby entry point for FastHammer.multi_sha1.
 *
 * Calculate meelyons of hashs and find the one with the lowest hamming
 * distance to another hash value.
 *
 * Return something that looks like this:
 * { :best_string => 'blah blah blah blah ... abcde', :hamming_distance => 17, :interupted => false, :out_of_time => true, :last_suffix => 'xyzab' }
 */
VALUE meth_FastHammer_multi_sha1
    (VALUE self, VALUE rb_match_hash, VALUE rb_base_string, VALUE start_suffix,
     VALUE end_suffix, VALUE max_time)
{
  const uint8_t *match_hash = (uint8_t *) RSTRING_PTR(rb_match_hash);
  uint8_t hash[20];
  SHA1_CTX context, sub_context;
  uint32_t lowest_hammer;
  char *hammer_string;
  size_t max_size;
  char *base_string;
  VALUE rc_hash;

  SHA1_Init(&context);
  SHA1_Update(&context, (uint8_t *) RSTRING_PTR(rb_base_string), RSTRING_LEN(rb_base_string));
  sub_context = context;

#define START_DEPTH (5)
  max_size = RSTRING_LEN(rb_base_string) + START_DEPTH + 2;
  hammer_string = malloc(max_size);
  base_string = malloc(max_size);
  if (hammer_string == NULL || base_string == NULL) {
    rb_raise(rb_const_get(rb_cObject, rb_intern("NoMemoryError")),
             "%s", "no memory for temporary strings");
    return Qnil; /* should never see this */
  }

  /* start with the base string */
  SHA1_Finish(&context, hash);
  lowest_hammer = internal_fast_hammer(match_hash, hash, 20);
  strcpy(hammer_string, StringValueCStr(rb_base_string));

  /* get the base context for the string with space */
  SHA1_Update(&sub_context, (uint8_t *) " ", 1);

  sprintf(base_string, "%s ", hammer_string);
  multi_sha1_ripper(match_hash, base_string,
                    base_string + RSTRING_LEN(rb_base_string) + 1,
                    &sub_context, hammer_string, &lowest_hammer, START_DEPTH);
  free(base_string);

  rc_hash = rb_hash_new();
  rb_hash_aset(rc_hash, ID2SYM(rb_intern("best_string")),
               rb_str_new2(hammer_string));
  rb_hash_aset(rc_hash, ID2SYM(rb_intern("hamming_distance")),
               LONG2FIX(lowest_hammer));
  rb_hash_aset(rc_hash, ID2SYM(rb_intern("interupted")), Qfalse);
  rb_hash_aset(rc_hash, ID2SYM(rb_intern("out_of_time")), Qfalse);
  rb_hash_aset(rc_hash, ID2SYM(rb_intern("last_suffix")),
               rb_str_new2("~~~~~"));

  free(hammer_string);
  return rc_hash;
}


void Init_fast_hammer (void)
{
  VALUE m_FastHammer = rb_define_module("FastHammer");
  rb_define_singleton_method(m_FastHammer, "fast_hammer",
                             meth_FastHammer_fast_hammer, 2);
  rb_define_singleton_method(m_FastHammer, "multi_sha1",
                             meth_FastHammer_multi_sha1, 5);
}
