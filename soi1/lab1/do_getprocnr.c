PUBLIC int do_getprocnr() {
    int caller_pid = mm_in.m2_l2;
    int i = 0;
    while (i < NR_PROCS) {
        if(mproc[i].mp_flags & IN_USE){
            if(mproc[i].mp_pid == caller_pid){
                return i;
            }
        }
        i++;
    }
    return ENOENT;
}