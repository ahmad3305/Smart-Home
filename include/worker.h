#ifndef WORKER_H
#define WORKER_H

#include "mystr.h"
using namespace std;

class Worker {
    Mystr workerId;
    Mystr workerName;
    Mystr contactInfo;
public:
    Worker();
    Worker(const Mystr& id, const Mystr& name, const Mystr& contactInfo);

    Mystr getWorkerId() const;
    Mystr getWorkerName() const;
    Mystr getContactInfo() const;
    void setWorkerName(const Mystr& name);
    void setContactInfo(const Mystr& info);
};

#endif