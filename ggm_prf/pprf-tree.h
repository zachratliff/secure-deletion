#ifndef PPRF_TREE
#define PPRF_TREE

#include <linux/types.h>


#define PRG_INPUT_LEN 16

u8 aes_input[2*PRG_INPUT_LEN] = "\000\001\002\003\004\005\006\007"
								"\010\011\012\013\014\015\016\017"
								"\020\021\022\023\024\025\026\027"
								"\030\031\032\033\034\035\036\037";

u8 iv[PRG_INPUT_LEN];
struct scatterlist sg_in;
struct crypto_blkcipher *tfm;

// This is arbitrary. it can support 2^64 inodes. Currently supporting anything larger would involve some rewriting
#define MAX_DEPTH 64 
#define NODE_LABEL_LEN (MAX_DEPTH+7)/8

u8 pprf_depth = MAX_DEPTH;

typedef struct node_label {
	u8 bstr[NODE_LABEL_LEN];
	u8 depth;
} node_label;

/* Binary-tree based organization of the PPRF keys
 * 	
 * We lay out the tree in an array consisting of
 * 	{il, ir, key[len]} triples
 * il: index where the left child is stored
 * ir: index where the right child is stored
 * key: value of the PPRF key (only meaningful for leaf nodes)
 * 
 * two sentinel indices
 * 0: this is a leaf node
 * -1: the subtree has been punctured
 * 
 * root node is placed at index 0
 * 
 * In particular I /don't/ think we need to store depth information
 * in this implementation because the depth matches the depth in the
 * tree exactly
 * 
 */

typedef struct pprf_keynode {
	u32 il;
	u32 ir;
	u8 key[PRG_INPUT_LEN];
#ifdef DEBUG
	node_label lbl;
#endif
} pprf_keynode;

pprf_keynode* master_key;
int master_key_count; // how many individual keys make up the master key
int max_master_key_count;



void reset_pprf_keynode(pprf_keynode *node);
void print_pkeynode_debug(node_label* lbl);
static inline void ggm_prf_get_random_bytes_kernel(u8 *data, u64 len);

int prg_from_aes_ctr(u8* key, u8* buf);

bool check_bit_is_set(u8* buf, u8 index);
void set_bit_in_buf(u8* buf, u8 index, bool val);

int alloc_master_key(void);
#define EXPANSION_FACTOR 4
int expand_master_key(void);
void init_master_key(void);

void init_node_label_from_bitstring(node_label *lbl, const char* bitstring);
void init_node_label_from_long(struct node_label *lbl, u64 val);

pprf_keynode *find_key(node_label *lbl, u32 *depth);
int puncture(node_label *lbl);
int puncture_at_tag(u64 tag);
int evaluate(node_label *lbl, u8 *out);
int evaluate_at_tag(u64 tag, u8* out);




#endif