#ifndef SETTING_HPP
#define SETTING_HPP

class Setting {
public:
  double p0;
  double rho0;
  double alpha;
  double kappa;
  double CFL;
  double M2is;

  Setting() {};
  ~Setting() {};
};

#endif
