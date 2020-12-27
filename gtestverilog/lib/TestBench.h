//
// based on https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html
//

#include <cstdint>
#include <memory>

namespace gtestverilog {

	/// @class TestBench
	/// @brief Templated base class for simulating a verilated verilog module
	template<class MODULE>
	class TestBench {
	public:
		TestBench(void) {
			m_core = std::make_unique<MODULE>();
			
			m_stepCount = 0;
			
			m_core->i_reset_n = 1;
			m_core->i_clk = 0;
			m_core->eval();
		}

		~TestBench(void) {
			m_core.release();
		}

		/// @brief set the 'i_reset_n' port low and simulate a clock tick
		void reset(void) {
			m_core->i_reset_n = 0;
			tick();
			m_core->i_reset_n = 1;
		}

		/// @brief simulate a clock cycle
		/// @note a clock cycle has two steps - first step has port 'i_clk'=1, the second step has port 'i_clk'=0
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

		/// @brief simulate a single half clock step
		/// @note This will invert the current value on port 'i_clk'
		void nextStep() {
			m_core->i_clk = (m_core->i_clk) ? 0 : 1;
			m_core->eval();

			m_stepCount += 1;

			onNextStep();
		}

		MODULE& core() {
			return *m_core;
		}

		uint64_t stepCount() const {
			return m_stepCount;
		}

	protected:
		virtual void onNextStep() {

		}

	private:
		uint64_t    m_stepCount;
		std::unique_ptr<MODULE>		m_core;

	};

}