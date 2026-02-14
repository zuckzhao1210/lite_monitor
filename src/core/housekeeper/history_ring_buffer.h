/**
*****************************************************************************
*  @file    history_ring_buffer.h
*  @brief   历史数据环形缓冲区
*
*  @author  zuckzhao
*  @date    2026-02-09
*  @version V0.1.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note V0.1.0   zuckzhao  2026-02-09   1.创建
*
*****************************************************************************
*/
#ifndef HISTORY_RING_BUFFER_H
#define HISTORY_RING_BUFFER_H

#include <deque>
#include <mutex>
#include <vector>
#include <condition_variable>

#pragma once

class HistoryRingBuffer {
public:
    HistoryRingBuffer() = default;
    ~HistoryRingBuffer() = default;
};

#endif