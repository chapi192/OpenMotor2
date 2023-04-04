#include <vector>
using std::vector;
#include <memory>
using std::unique_ptr;

struct GrainData {
	vector<float> m_mass;
	vector<float> m_massFlow;
	vector<float> m_massFlux;
	vector<float> m_reg;
	vector<float> m_web;
};

struct SimData {
public:
	SimData(std::size_t grainCount,
			unique_ptr<float>& mass,
			unique_ptr<float>& web,
			unique_ptr<float>& massFlow,
			unique_ptr<float>& massFlux,
			unique_ptr<float>& reg
	) :
		m_grainCount{ grainCount },
		m_grains{ grainCount },
		m_massArray{ mass },
		m_massFlowArray{ massFlow },
		m_massFluxArray{ massFlux },
		m_regArray{ reg },
		m_webArray{ web } {}

	/** t = 0 */
	void init(
			float kn,
			float pressure,
			float volumeLoading
	) {
		m_time         .push_back(0);
		m_kn           .push_back(kn);
		m_pressure     .push_back(pressure);
		m_force        .push_back(0);
		m_volumeLoading.push_back(volumeLoading);
		m_exitPressure .push_back(0);
		m_dThroat      .push_back(0);
	}

	void pushGrainArrays() {
		auto mass = m_massArray.get();
		auto massFlow = m_massFlowArray.get();
		auto massFlux = m_massFluxArray.get();
		auto reg = m_regArray.get();
		auto web = m_webArray.get();

		for (size_t i = 0; i < m_grainCount; i++) {
			m_grains[i].m_mass    .push_back(*mass++);
			m_grains[i].m_massFlow.push_back(*massFlow++);
			m_grains[i].m_massFlux.push_back(*massFlux++);
			m_grains[i].m_reg     .push_back(*reg++);
			m_grains[i].m_web     .push_back(*web++);
		}
	}
public:
	const size_t m_grainCount;

	vector<float>  m_time;
	vector<float>  m_kn;
	vector<float>  m_pressure;
	vector<float>  m_force;
	vector<float>  m_volumeLoading;
	vector<float>  m_exitPressure;
	vector<float>  m_dThroat;  // the change in throat diameter due to erosion or slag accumulation.

	vector<GrainData> m_grains;
private:
	unique_ptr<float>& m_massArray;
	unique_ptr<float>& m_massFlowArray;
	unique_ptr<float>& m_massFluxArray;
	unique_ptr<float>& m_regArray;
	unique_ptr<float>& m_webArray;
};