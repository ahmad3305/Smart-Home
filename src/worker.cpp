#include "../include/Worker.h"

Worker::Worker() : workerId(""), workerName(""), contactInfo("") {}
Worker::Worker(const Mystr& id, const Mystr& name, const Mystr& contactInfo)
    : workerId(id), workerName(name), contactInfo(contactInfo) {
}

Mystr Worker::getWorkerId() const { return workerId; }
Mystr Worker::getWorkerName() const { return workerName; }
Mystr Worker::getContactInfo() const { return contactInfo; }
void Worker::setWorkerName(const Mystr& name) { workerName = name; }
void Worker::setContactInfo(const Mystr& info) { contactInfo = info; }