// LinearVelocityEngine.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <functional>
#include <vector>
#include <chrono>

#include <immintrin.h>
#include <immintrin.h>

using namespace std;

union Info {
	__m128 particlesSIMD;
	float particlesF[4];
};

class E19AlexCanut {
public:
	vector<Info> p;
	vector<Info> v;

	E19AlexCanut() {

	}

	//vector<Particle> particles;
	int counter = 0;

	//Add a particle to the system
	//vy	vx	y	x
	void Add(float px, float py, float vx, float vy) {
		//USING SIMD
		if (counter == 0) {
			p.push_back({ px, py, 0.0f, 0.0f });
			v.push_back({ vx, vy, 0.0f,0.0f });
		}
		else {
			p[p.size() - 1].particlesF[2] = px;
			p[p.size() - 1].particlesF[3] = py;

			v[p.size() - 1].particlesF[2] = vx;
			v[p.size() - 1].particlesF[3] = vy;
		}
		++counter;
		counter %= 2;
	}

	//Update all the particles
	void Update(float dt) {
		__m128 dtV = _mm_set_ps(dt, dt, dt, dt);
		for (int i = 0; i < p.size(); i++) {
			p[i].particlesSIMD = _mm_fmadd_ps(v[i].particlesSIMD, dtV, p[i].particlesSIMD);
		}
	}

	void Run(std::function<void(float x, float y)> f) {
		for (int i = 0; i < p.size(); i++) {
			f(p[i].particlesF[0], p[i].particlesF[1]);
			f(p[i].particlesF[2], p[i].particlesF[3]);
		}
	}
};

int main() {
	E19AlexCanut* a = new E19AlexCanut();

	/*BODY*/
	for (int i = 0; i < 15000000; i++) a->Add(1.0f, 2.0f, 20.0f, 21.0f);
	auto start = chrono::high_resolution_clock::now();
	for(int i = 0; i<60; i++) a->Update(10.0f);
	
	/*END BODY*/

	auto end = chrono::high_resolution_clock::now();
	auto finalT = chrono::duration_cast<chrono::milliseconds> (end - start);
	cout << finalT.count() << " ms" << endl;
	cout << a->p[0].particlesF[0] <<" "<< a->p[0].particlesF[1] << endl;
	//cout << a->p.size() << endl;
	//cout << a->particles[0].valuesF[0] << ", " << a->particles[0].valuesF[1] << ", " << a->particles[0].valuesF[2] << ", " << a->particles[0].valuesF[3] << endl;
	//cout << a->particles[0].valuesF[0] << ", " << a->particles[0].valuesF[1] << endl;
	
	return 1;
}