#include <iostream>
#include <limits>
#include <vector>
#include <cmath>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
        double m_min;
    public:
        Min() : m_min{std::numeric_limits<double>::min()} {
        }

        void update(double next) override {
            if (next < m_min) {
                m_min = next;
            }
        }

        double eval() const override {
            return m_min;
        }

        const char * name() const override {
            return "min";
	    }
};

class Max : public IStatistics {
        double m_max;
    public:
        Max() : m_max{std::numeric_limits<double>::max()} {
        }

        void update(double next) override {
            if (next > m_max) {
                m_max = next;
            }
        }

        double eval() const override {
            return m_max;
        }

        const char * name() const override {
            return "max";
        }

};


class Mean : public IStatistics
{
    unsigned int m_amount;
    double m_sum;
	double m_mean;

public:
	Mean(void);
	void update(double next) override;
	double eval() const override;
	const char * name() const override;
};


Mean::Mean() :
	m_amount{0}, m_sum{0.0}, m_mean{0.0}
{

}

void Mean::update(double next)
{
	m_amount++;
	m_sum += next;
	m_mean = m_sum / static_cast<double>(m_amount);

	return;
}

double Mean::eval() const
{
	return this->m_amount > 0 ? this->m_mean : 0.0;
}

const char* Mean::name() const
{
	return "mean";
}


class Std : public IStatistics
{
private:
    unsigned int m_amount;
    double m_sum;
	double m_mean;
    double m_sumSquared;
	double m_std;

public:
	Std(void);

	void update(double next) override;
	double eval() const override;
	const char * name() const override;
};


Std::Std() :
	m_amount{0}, m_sum{0.0}, m_mean{0.0}, m_sumSquared{0.0}, m_std{0.0}
{

}

void Std::update(double next)
{
	m_amount++;
	m_sum += next;
	m_sumSquared += next * next;
	m_mean = m_sum / static_cast<double>(m_amount);

	return;
}

double Std::eval() const
{
	/* STD is undefined for one number */
	if(m_amount < 2)
	{
		return 0.0;
	}

	double variance = (m_sumSquared - m_amount * m_mean * m_mean) / m_amount;
	return std::sqrt(variance);
}

const char* Std::name() const
{
	return "std";
}



void FindingTheMinimumValue(std::vector<double> &results){
/* 	size_t statistics_count = results.size();
	IStatistics *statistics = new IStatistics statistics_count; */

	const size_t statistics_count = 1;
	IStatistics *statistics[statistics_count];	
	statistics[0] = new Min{};

	for (int it = 0; it < results.size(); it++) {
		for (auto i = 0; i < statistics_count; i++) {
			statistics[i] -> update(results[it]);
		}
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

}

void FindingTheMaximumValue(std::vector<double> &results){
/* 	size_t statistics_count = results.size();
	IStatistics *statistics[statistics_count]; */
	const size_t statistics_count = 1;
	IStatistics *statistics[statistics_count];	
	statistics[0] = new Max{};

	for (int it = 0; it < results.size(); it++) {
		for (auto i = 0; i < statistics_count; i++) {
			statistics[i]->update(results[it]);
		}
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

}

void FindingTheMeanValue(std::vector<double> &results){
	// size_t statistics_count = results.size();
	// IStatistics *statistics[statistics_count];
	const size_t statistics_count = 1;
	IStatistics *statistics[statistics_count];	
	statistics[0] = new Mean{};

	for (int it = 0; it < results.size(); it++) {
		for (auto i = 0; i < statistics_count; i++) {
			statistics[i]->update(results[it]);
		}
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

}

void FindingTheStdValue(std::vector<double> &results){
	// size_t statistics_count = results.size();
	// IStatistics *statistics[statistics_count];
	const size_t statistics_count = 1;
	IStatistics *statistics[statistics_count];	
	statistics[0] = new Std{};

    for (auto i = 0; i < statistics_count; i++) {
        statistics[i] -> update(results[i]);
    }

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

}

int RanStatistics(std::vector<double> &results) {
    FindingTheMinimumValue(results);
    FindingTheMaximumValue(results);
    FindingTheMeanValue(results);
    FindingTheStdValue(results);

	return 0;
}