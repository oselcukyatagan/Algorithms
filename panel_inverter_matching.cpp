#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>


using namespace std;

class Panel {
private:
    double maxPower;
    double Voc;
    double Isc;
    double Vmax;
    double Imax;
    double alphaPower;
    double alphaVoc;
    double alphaIsc;
    double maxTemp;
    double minTemp;
public:
    Panel(double p, double _voc, double _isc, double _vmax, double _imax,
          double _alphaPower, double _alphaVoc, double _alphaIsc, double _maxTemp, double _minTemp)
        : maxPower(p), Voc(_voc), Isc(_isc), Vmax(_vmax), Imax(_imax),
          alphaPower(_alphaPower), alphaVoc(_alphaVoc), alphaIsc(_alphaIsc), maxTemp(_maxTemp), minTemp(_minTemp){}

    double getMaxPower() const { return maxPower; }
    double getVoc() const { return Voc; }
    double getIsc() const { return Isc; }
    double getVmax() const { return Vmax; }
    double getImax() const { return Imax; }
    double getMaxTemp() const { return maxTemp; }
    double getMinTemp() const { return minTemp; }
    double getColdVoc() const {
    return Voc * (1 + alphaVoc * (minTemp - 25));
    }
    double getHotVoc() const {
        return Voc * (1 + alphaVoc * (maxTemp - 25));
    }


    void printInfo() const {
        std::cout << "Panel -> Max Power: " << maxPower
                  << " W, Voc: " << Voc
                  << " V, Isc: " << Isc
                  << " A, Vmax: " << Vmax
                  << " V, Imax: " << Imax
                  << " A, alphaPower: " << alphaPower
                  << ", alphaVoc: " << alphaVoc
                  << ", alphaIsc: " << alphaIsc
                  << "\n";
    }
};


class Inverter {
private:
    int maxInputVoltage;
    int minInputVoltage;
    int ratedInputVoltage;
    int mpptVoltageLow;
    int mpptVoltageHigh;
    int numberOfMPPTs;
    int stringPerMPPT;
    int maxInputCurrentPerMppt;
    int maxScCurrentPerMppt;

    int maxACOutputPower;
    double maxACOutputCurrent;
    double ratedACOutputCurrent;
    double ratedACOutputVoltage;
    

public:
    Inverter(int _maxInputVoltage, int _minInputVoltage, int _ratedInputVoltage,
             int _mpptVoltageLow, int _mpptVoltageHigh, int _numberOfMPPTs,
             int _stringPerMPPT, int _maxInputCurrent, int _maxScCurrent,
             int _maxACOutputPower, double _maxACOutputCurrent,
             double _ratedACOutputCurrent, double _ratedACOutputVoltage)
        : maxInputVoltage(_maxInputVoltage), minInputVoltage(_minInputVoltage),
          ratedInputVoltage(_ratedInputVoltage), mpptVoltageLow(_mpptVoltageLow),
          mpptVoltageHigh(_mpptVoltageHigh), numberOfMPPTs(_numberOfMPPTs),
          stringPerMPPT(_stringPerMPPT), maxInputCurrentPerMppt(_maxInputCurrent),
          maxScCurrentPerMppt(_maxScCurrent), maxACOutputPower(_maxACOutputPower),
          maxACOutputCurrent(_maxACOutputCurrent), ratedACOutputCurrent(_ratedACOutputCurrent),
          ratedACOutputVoltage(_ratedACOutputVoltage) {}

          // Voltage
        int getMaxInputVoltage() const { return maxInputVoltage; }
        int getMinInputVoltage() const { return minInputVoltage; }
        int getRatedInputVoltage() const { return ratedInputVoltage; }
        int getMpptVoltageLow() const { return mpptVoltageLow; }
        int getMpptVoltageHigh() const { return mpptVoltageHigh; }

        // MPPT & strings
        int getNumberOfMPPTs() const { return numberOfMPPTs; }
        int getStringPerMPPT() const { return stringPerMPPT; }

        // Currents
        int getMaxInputCurrent() const { return maxInputCurrentPerMppt; }
        int getMaxScCurrent() const { return maxScCurrentPerMppt; }

        // AC output
        int getMaxACOutputPower() const { return maxACOutputPower; }
        double getMaxACOutputCurrent() const { return maxACOutputCurrent; }
        double getRatedACOutputCurrent() const { return ratedACOutputCurrent; }
        double getRatedACOutputVoltage() const { return ratedACOutputVoltage; }


};

int maxPanelSeries(const Panel& panel, const Inverter& inverter) {
    int a = inverter.getMaxInputVoltage();
    double b = panel.getColdVoc();
    return floor(a / b);
}

int maxPanelParallel(const Panel& panel, const Inverter& inverter){
    return inverter.getStringPerMPPT() * inverter.getNumberOfMPPTs();
}

int ratedPanelSeries(const Panel& panel, const Inverter& inverter){
    return ceil(inverter.getRatedInputVoltage() / panel.getVmax());
}

int currentPerMppt(const Panel& panel, const Inverter& inverter){
    return inverter.getStringPerMPPT() * panel.getImax();
}

bool checkIsc(const Panel& panel, const Inverter& inverter){
    if(inverter.getMaxScCurrent() > panel.getIsc() * inverter.getStringPerMPPT()){
        return true;
    }
    return false;
}

double VmpRated(const Panel& panel, const Inverter& inverter) {
    return ratedPanelSeries(panel, inverter) * panel.getVmax();
}

double ImpPerMpptRated(const Panel& panel, const Inverter& inverter) {
    return inverter.getStringPerMPPT() * panel.getImax();
}

double PdcRated(const Panel& panel, const Inverter& inverter) {
    return maxPanelParallel(panel, inverter) * VmpRated(panel, inverter) * panel.getImax();
}
 
double inOutRatioRated(const Panel& panel, const Inverter& inverter) {
    return PdcRated(panel, inverter) / inverter.getMaxACOutputPower();
}

void checkCompatibility(const Panel& panel, const Inverter& inverter){
    cout << "Max panels in series: " << maxPanelSeries(panel, inverter) << endl;
    cout << "Max panels in parallel: " << maxPanelParallel(panel, inverter) << endl;
    cout << "Number of panels in series to get a close result to nominal rating: " << ratedPanelSeries(panel, inverter) << endl;    
    
}

void checkCompatibilityRated(const Panel& panel, const Inverter& inverter){
    cout << "Rated Input Voltage Design" << endl;
    cout << "Vmp: " << VmpRated(panel, inverter) << "V" << endl;
    cout << "Imp: " << ImpPerMpptRated(panel, inverter) << "A" << endl;
    cout << "Pdc: " << fixed << setprecision(2) << (PdcRated(panel, inverter) / 1000.0) << " kW" << endl;
    cout << "DC/AC: " << inOutRatioRated(panel, inverter) << endl;
}


double PdcMax(const Panel& panel, const Inverter& inverter) {
    return (maxPanelSeries(panel,inverter) * panel.getVmax() * inverter.getStringPerMPPT() * 
    inverter.getNumberOfMPPTs() * panel.getImax() );
}
 
double inOutRatioMax(const Panel& panel, const Inverter& inverter) {
    return PdcMax(panel, inverter) / inverter.getMaxACOutputPower();
}


void checkCompatibilityMax(const Panel& panel, const Inverter& inverter){
    cout << "Max Input Voltage Design" << endl;
    cout << "Vmp: " << maxPanelSeries(panel,inverter) * panel.getVmax() << "V" << endl;
    cout << "Imp: " << ImpPerMpptRated(panel, inverter) << "A" << endl;
    cout << "Pdc: " << fixed << setprecision(2) << (PdcMax(panel,inverter) / 1000.0) << " kW" << endl;
    cout << "DC/AC: " << inOutRatioMax(panel, inverter) << endl;
}

 

int main() {
    // Create some panels
    Panel p1(447, 49.62, 11.7, 40.66, 11.01,
         -0.0032, -0.0026, 0.00046, 45, -10);
    

    // Create inverter
    Inverter inv(1100, 180, 600, 180, 1000, 7, 3, 48,
         66, 150000, 240.6, 216.5, 400); 

        
   
    checkCompatibility(p1,inv);
    
    checkCompatibilityRated(p1,inv);

    checkCompatibilityMax(p1,inv);
    

    return 0;
}
