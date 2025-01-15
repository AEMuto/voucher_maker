#pragma once

#include <memory>
using namespace std;

class MemoryMonitorBase {
 public:
  virtual ~MemoryMonitorBase() = default;
  virtual void begin() {}
};

#ifdef DEBUG
class MemoryMonitor : public MemoryMonitorBase {  // Inherit from MemoryMonitorBase
 private:
  // Track lowest ever free heap
  static size_t lowestHeap;
  static size_t lowestPSRAM;
  static unsigned long lastCheck;
  static const unsigned long CHECK_INTERVAL = 1000;  // Check every second

 public:
  struct MemoryStats {
    size_t freeHeap;
    size_t freePSRAM;
    size_t largestHeapBlock;
    size_t heapFragmentation;
    size_t lowestHeapSeen;
    size_t lowestPSRAMSeen;
  };

  // Get current memory statistics
  static MemoryStats getStats();

  // Check if memory is low
  static bool isMemoryLow(size_t threshold = 10000);

  // Print memory stats to Serial
  static void printStats(bool forceOutput = false);

  // Call this in setup()
  void begin() override;  // Override the base class method
};
#endif

unique_ptr<MemoryMonitorBase> createMemoryMonitor();
