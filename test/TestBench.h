//
// based on https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html
//

#include <cstdint>
#include <memory>

template<class MODULE>
class TestBench {
public:
	TestBench(void) {
		m_core = std::make_unique<MODULE>();
		m_tickCount = 0;
	}

	virtual ~TestBench(void) {
		m_core.release();
	}

	virtual void reset(void) {
		m_core->i_reset = 1;
		tick();
		m_core->i_reset = 0;
	}

    void tick(uint64_t count) {
        for (uint64_t i=0; i<count; i++) {
            tick();
        }
    }

	virtual void tick(void) {
		// Increment our own internal time reference
		m_tickCount++;

		// Make sure any combinatorial logic depending upon
		// inputs that may have changed before we called tick()
		// has settled before the rising edge of the clock.
		clockFallingEdge();

		// Toggle the clock

		clockRisingEdge();
		clockFallingEdge();
	}

	virtual void clockRisingEdge() {
		m_core->i_clk = 1;
		m_core->eval();
	}

	virtual void clockFallingEdge() {
		m_core->i_clk = 0;
		m_core->eval();
	}

    MODULE& core() {
        return *m_core;
    }

    uint64_t tickCount() const {
        return m_tickCount;
    }

private:
	uint64_t    m_tickCount;
	std::unique_ptr<MODULE>		m_core;

};
