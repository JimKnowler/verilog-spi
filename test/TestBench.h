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

		m_core->i_clk = 0;
		m_core->eval();
	}

	virtual ~TestBench(void) {
		m_core.release();
	}

	virtual void reset(void) {
		m_core->i_reset = 1;
		tick();
		m_core->i_reset = 0;
	}

	void tick(size_t numTicks = 1) {
		for (size_t i=0; i<numTicks; i++) {
			// rising edge
			assert(m_core->i_clk == 0);
			step();

			// falling edge
			assert(m_core->i_clk == 1);
			step();
		}
	}

	void step() {
		m_core->i_clk = (m_core->i_clk) ? 0 : 1;
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
