#pragma once

#include <atomic>
#include<QThread>

#include"rqw_LabelWarning.h"
#include"dsl_Heap.hpp"

#include"Utilty.hpp"


class DetachDefectThreadWetPapers : public QThread
{
	Q_OBJECT
public:
	std::atomic_bool isProcessing{ false };
	std::atomic_bool isProcessFinish{ false };
public:
	explicit DetachDefectThreadWetPapers(QObject* parent = nullptr);

	~DetachDefectThreadWetPapers() override;

	void startThread();

	void stopThread();

	void processQueue1(std::unique_ptr<rw::dsl::ThreadSafeHeap<float>>& queue);
	void processQueue2(std::unique_ptr<rw::dsl::ThreadSafeHeap<float>>& queue);

protected:
	void run() override;
private:
	std::atomic<bool> running; // 使用原子变量保证线程安全
};
