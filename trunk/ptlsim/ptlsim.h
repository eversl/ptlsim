// -*- c++ -*-
//
// PTLsim: Cycle Accurate x86-64 Simulator
// Simulator Structures
//
// Copyright 2000-2005 Matt T. Yourst <yourst@yourst.com>
//

#ifndef _PTLSIM_H_
#define _PTLSIM_H_

#include <globals.h>
#include <ptlhwdef.h>
#include <kernel.h>
#include <dcache.h>
#include <config.h>
#include <datastore.h>

extern W64 sim_cycle;

void user_process_terminated(int rc);

ostream& print_user_context(ostream& os, const UserContext& ctx, int width = 4);

void init_uops();
void init_translate();
BasicBlock* translate_basic_block(void* rip);
void capture_translate_timers(DataStoreNode& root);
void capture_translate_stats(DataStoreNode& root);
extern bool split_unaligned_memops_during_translate;

int out_of_order_core_toplevel_loop();
int sequential_core_toplevel_loop();

void ooo_capture_stats();
void ooo_capture_stats(DataStoreNode& root);
void seq_capture_stats(DataStoreNode& root);
void save_stats();

extern "C" void switch_to_sim();

//
// uop implementations
//

struct AddrPair {
  byte* start;
  byte* end;
};

uopimpl_func_t get_synthcode_for_uop(int op, int size, bool setflags, int cond, int extshift, int sfra, int cachelevel, bool except, bool internal);
uopimpl_func_t get_synthcode_for_cond_branch(int opcode, int cond, int size, bool except);
void synth_uops_for_bb(BasicBlock& bb);

void add_unaligned_ldst_rip(W64 rip);
void remove_unaligned_ldst_rip(W64 rip);
bool check_unaligned_ldst_rip(W64 rip);

extern Hashtable<W64, BasicBlock*, 16384> bbcache;

//
// Assists
//

typedef void (*assist_func_t)();

const char* assist_name(assist_func_t func);
int assist_index(assist_func_t func);
void update_assist_stats(assist_func_t assist);
void reset_assist_stats();
void save_assist_stats(DataStoreNode& root);


#endif // _PTLSIM_H_
