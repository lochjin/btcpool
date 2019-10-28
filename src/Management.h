
/*
 The MIT License (MIT)

 Copyright (c) [2019] [BTC.COM]

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */
#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <libconfig.h++>
#include <nlohmann/json.hpp>
#include "Kafka.h"

using JSON = nlohmann::json;
using JSONException = nlohmann::detail::exception;

class StratumServer;

class Management {
protected:
  std::atomic<bool> running_;
  string chainType_;

  bool autoSwitchChain_ = false;
  std::atomic<size_t> currentAutoChainId_;

  KafkaSimpleConsumer controllerTopicConsumer_;
  KafkaProducer processorTopicProducer_;
  std::thread consumerThread_;
  StratumServer &server_;

  void sendMessage(string msg);
  void sendOnlineNotification();
  void sendOfflineNotification();
  void sendExceptionReport(const std::exception &ex);
  JSON getConfigureAndStatus(const string &type, const string &action);

public:
  Management(const libconfig::Config &cfg, StratumServer &server_);
  bool setup();
  void run();
  void stop();
  void handleMessage(rd_kafka_message_t *rkmessage);
};