//
// based on https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html
//

#include <cstdint>
#include <memory>

namespace gtestverilog {

	template<class MODULE>
	class TestBench {
	public:
		TestBench(void) {
			m_core = std::make_unique<MODULE>();
			
			m_stepCount = 0;

			m_core->i_clk = 0;
			m_core->eval();
		}

		~TestBench(void) {
			m_core.release();
		}

		void reset(void) {
			m_core->i_reset = 1;
			tick();
			m_core->i_reset = 0;
		}

		void tick(size_t numTicks = 1) {
			for (size_t i=0; i<numTicks; i++) {
				// rising edge
				assert(m_core->i_clk == 0);
				nextStep();

				// falling edge
				assert(m_core->i_clk == 1);
				nextStep();
			}
		}

		void nextStep() {
			m_core->i_clk = (m_core->i_clk) ? 0 : 1;
			m_core->eval();

			m_stepCount += 1;

			onNextStep();
		}

		virtual void onNextStep() {

		}

		MODULE& core() {
			return *m_core;
		}

		uint64_t stepCount() const {
			return m_stepCount;
		}

	private:
		uint64_t    m_stepCount;
		std::unique_ptr<MODULE>		m_core;

	};

}