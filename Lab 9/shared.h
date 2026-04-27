struct SharedMemory
{
    int frameNumber;
    int totalFrames;
    int newFrame;
    char frame[4096];       //frame size
}

void wait(int semID)
{
    struct sembuf op{};
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = SEM_UNDO;
    semop(semId, &op, 1);
}

void signal(int semID)
{
    struct sembuf op{};
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = SEM_UNDO;
    semop(semId, &op, 1);
}