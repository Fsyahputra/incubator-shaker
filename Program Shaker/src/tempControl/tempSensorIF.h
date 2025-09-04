//
// Created by fadhil on 9/4/25.
//

#ifndef TEMPSENSORIF_H
#define TEMPSENSORIF_H

class tempIF {
public:
    virtual ~tempIF() = default;
    virtual void init();
    virtual float readTemperatureCelsius() = 0;
};

#endif //TEMPSENSORIF_H