#ifndef C_DANGEON_H_INCLUDED
#define C_DANGEON_H_INCLUDED

#define CAT( L, R )  L##R
#define DECLARE(struct_, typename)  CAT (struct_##_, typename)

#define RETURN_IF_TRUE( cond, err )  { if (cond) return err; };

#define TO_STRING( name )  #name

#define MIN( l, r )  (((l) < (r)) ? (l) : r)
#define MAX( l, r )  (((l) > (r)) ? (l) : r)

#endif