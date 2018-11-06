PUBLIC int do_addnrtopid() {
  int caller_pid = mproc[mm_in.m_source].mp_pid;
  int arg = mm_in.m2_l2;
  return caller_pid + arg;
}
