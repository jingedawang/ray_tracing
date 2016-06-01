#include "raytracing.h"

Solid::Solid(
	const Vec_3f &sc,
	const float &refl,
	const float &transp,
	const Vec_3f &ec) :
	surfaceColor(sc), reflection(refl), transparency(transp), emissionColor(ec)
{}

Sphere::Sphere(
	const Vec_3f &c,
	const float &r,
	const Vec_3f &sc,
	const float &refl,
	const float &transp,
	const Vec_3f &ec) :
	center(c), radius(r), radius2(r * r), Solid(sc, refl, transp, ec)
{}

bool Sphere::intersect(const Vec_3f &rayorigin, const Vec_3f & raydirection, float &t0, float &t1) const
{
	Vec_3f l = center - rayorigin;
	//Determine whether reverse direction 
	float tca = l.dot(raydirection);
	if (tca < 0) return false;
	//a^2=b^2+c^2
	float dist = l.dot(l) - tca * tca;
	if (dist > radius2) return false;
	float thc = sqrt(radius2 - dist);
	//t0: first intersection distance, t1: second intersection distance
	t0 = tca - thc;
	t1 = tca + thc;

	return true;
}

Vec_3f Sphere::nhit(const Vec_3f &phit) const
{
	//normal of the intersection point
	Vec_3f nhit = phit - center;
	return nhit;
}

Cube::Cube(
	const Vec_3f & vertex,
	const float & xLength,
	const float & yLength,
	const float & zLength,
	const Vec_3f &sc,
	const float & refl,
	const float & transp) :
	vertex(vertex), xLength(xLength), yLength(yLength), zLength(zLength), Solid(sc, refl, transp)
{}

bool Cube::intersect(const Vec_3f &rayorigin, const Vec_3f & raydirection, float &t0, float &t1) const
{
	/*float distance1 = abs(rayorigin.x - vertex.x) / abs(Vec_3f(1, 0, 0).dot(raydirection) / raydirection.length());
	float distance2 = abs(rayorigin.x - vertex.x - xLength) / abs(Vec_3f(1, 0, 0).dot(raydirection) / raydirection.length());
	float t0min = distance1 > distance2 ? distance2 : distance1;
	float t0max = distance1 > distance2 ? distance1 : distance2;
	distance1 = abs(rayorigin.y - vertex.y) / abs(Vec_3f(0, 1, 0).dot(raydirection) / raydirection.length());
	distance2 = abs(rayorigin.y - vertex.y - yLength) / abs(Vec_3f(0, 1, 0).dot(raydirection) / raydirection.length());
	float t1min = distance1 > distance2 ? distance2 : distance1;
	float t1max = distance1 > distance2 ? distance1 : distance2;
	distance1 = abs(rayorigin.z - vertex.z) / abs(Vec_3f(0, 0, 1).dot(raydirection) / raydirection.length());
	distance2 = abs(rayorigin.z - vertex.z - zLength) / abs(Vec_3f(0, 0, 1).dot(raydirection) / raydirection.length());
	float t2min = distance1 > distance2 ? distance2 : distance1;
	float t2max = distance1 > distance2 ? distance1 : distance2;
	float temp = t0min > t1min ? t0min : t1min;
	float tmin = temp > t2min ? temp : t2min;
	temp = t0max < t1max ? t0max : t1max;
	float tmax = temp < t2max ? temp : t2max;
	if (tmin < tmax)
	{
		t0 = tmin;
		t1 = tmax;
		return true;
	}
	return false;*/

	float t01 = -(vertex.x + Vec_3f(-1, 0, 0).dot(rayorigin)) / Vec_3f(-1, 0, 0).dot(raydirection);
	float t02 = -(-vertex.x - xLength + Vec_3f(1, 0, 0).dot(rayorigin)) / Vec_3f(1, 0, 0).dot(raydirection);
	float t11 = -(vertex.y + Vec_3f(0, -1, 0).dot(rayorigin)) / Vec_3f(0, -1, 0).dot(raydirection);
	float t12 = -(-vertex.y - yLength + Vec_3f(0, 1, 0).dot(rayorigin)) / Vec_3f(0, 1, 0).dot(raydirection);
	float t21 = -(vertex.z + Vec_3f(0, 0, -1).dot(rayorigin)) / Vec_3f(0, 0, -1).dot(raydirection);
	float t22 = -(-vertex.z - zLength + Vec_3f(0, 0, 1).dot(rayorigin)) / Vec_3f(0, 0, 1).dot(raydirection);

	float t0min = t01 < t02 ? t01 : t02;
	float t0max = t01 > t02 ? t01 : t02;
	float t1min = t11 < t12 ? t11 : t12;
	float t1max = t11 > t12 ? t11 : t12;
	float t2min = t21 < t22 ? t21 : t22;
	float t2max = t21 > t22 ? t21 : t22;

	float temp = t0min > t1min ? t0min : t1min;
	float tmin = temp > t2min ? temp : t2min;
	temp = t0max < t1max ? t0max : t1max;
	float tmax = temp < t2max ? temp : t2max;

	if (tmin < 0 && tmax < 0)
	{
		return false;
	}

	if (tmin < tmax)
	{
		t0 = tmin;
		t1 = tmax;
		return true;
	}
	return false;

}

Vec_3f Cube::nhit(const Vec_3f &phit) const
{
	float distance[6];
	distance[0] = abs(phit.x - vertex.x);
	distance[1] = abs(phit.x - vertex.x - xLength);
	distance[2] = abs(phit.y - vertex.y);
	distance[3] = abs(phit.y - vertex.y - yLength);
	distance[4] = abs(phit.z - vertex.z);
	distance[5] = abs(phit.z - vertex.z - zLength);
	int minIndex = 0;
	for (int i = 1; i < 6; i++)
	{
		if (distance[i] < distance[minIndex])
		{
			minIndex = i;
		}
	}
	switch (minIndex)
	{
	case 0:
		return Vec_3f(-1, 0, 0);
		break;
	case 1:
		return Vec_3f(1, 0, 0);
		break;
	case 2:
		return Vec_3f(0, -1, 0);
		break;
	case 3:
		return Vec_3f(0, 1, 0);
		break;
	case 4:
		return Vec_3f(0, 0, -1);
		break;
	case 5:
		return Vec_3f(0, 0, 1);
		break;
	default:
		return Vec_3f();
		break;
	}

}

//Calculate the mix value for reflection and refraction
float mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

//Ray Tracing Function: takes a ray (defined by its origin and direction) as argument.
//Through the function, we can know if the ray intersects any of the geometry in the scene.
//If the ray intersects an object, calculate the intersection point and its normal, then shade the point.
//Shading depends on the surface (transparent, reflective, diffuse)
//If the ray intersects an object, then return the color of the object at the intersection point, otherwise return the backgroud color.
Vec_3f trace(
	const Vec_3f &rayorigin,
	const Vec_3f &raydirection,
	const std::vector<Solid*> &solids,
	const int &depth
	)
{
	float tnear = INFINITY;
	const Solid* solid = NULL;
	//calculate intersection of this ray with the sphere in the scene
	for (unsigned i = 0; i < solids.size(); i++)
	{
		float t0 = INFINITY;
		float t1 = INFINITY;
		if (solids[i]->intersect(rayorigin, raydirection, t0, t1))
		{
			if (t0 < 0 && t1 < 0) {
				int fefef = 1;
			}
			//If the point in the sphere
			if (t0 < 0) t0 = t1;
			if (t0 < tnear)
			{
				tnear = t0;
				solid = solids[i];
			}
		}
	}

	//If there is no intersection, then return backgroud color
	if (!solid) return Vec_3f(1);
	//Color of ray
	Vec_3f surfaceColor = 0;
	//point of intersect
	Vec_3f phit = rayorigin + raydirection * tnear;
	
	Vec_3f nhit = solid->nhit(phit);
	//normalize the normal direction
	nhit.normal();
	//If the normal and the view direction's dot is positive, means the view point inside sphere
	float bias = 1e-4;
	bool inside = false;
	if (raydirection.dot(nhit) > 0)
	{
		nhit = -nhit;
		inside = true;
	}
	//Tackle with relection and refraction
	if ((solid->transparency > 0 || solid->reflection > 0) && depth < MAX_DEPTH)
	{
		//Compute fresnel effect
		float facingratio = -raydirection.dot(nhit);
		float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
		//Compute reflection direction
		Vec_3f reflect_direction = raydirection - nhit * 2 * raydirection.dot(nhit);
		reflect_direction.normal();
		Vec_3f next_reflection = trace(phit + nhit * bias, reflect_direction, solids, depth + 1);
		//Vec_3f next_reflection = trace(phit, reflect_direction, spheres, depth + 1);
		Vec_3f next_refraction = 0;
		//Only if the sphere is transparent, then compute refraction ray
		if (solid->transparency)
		{
			//judge whether we are inside or outside? ior is the index of two materials ÕÛÉäÂÊ
			float ior = 1.1, eta = (inside) ? ior : 1 / ior;
			float cosi = -nhit.dot(raydirection);
			float k = 1 - eta * eta * (1 - cosi * cosi);
			Vec_3f refraction_direction = raydirection * eta + nhit * (eta * cosi - sqrt(k));
			refraction_direction.normal();
			next_refraction = trace(phit - nhit * bias, refraction_direction, solids, depth + 1);
			//next_refraction = trace(phit, refraction_direction, spheres, depth+1);           
		}
		//The surface is a mix of reflection and refraction (if the sphere is transparent)
		surfaceColor = (next_reflection * fresneleffect + next_refraction * (1 - fresneleffect) * solid->transparency) * solid->surfaceColor;
	}
	//If it is a diffuse object, no need to ray tracing.
	else
	{
		for (unsigned i = 0; i < solids.size(); i++)
		{
			//This is a light
			if (solids[i]->emissionColor.x > 0)
			{
				Vec_3f transmission = 1;
				Vec_3f lightDirection = ((Sphere*)solids[i])->center - phit;
				lightDirection.normal();
				//Check whether have an obstacle between light and object, add shadow
				for (unsigned j = 0; j < solids.size(); ++j)
				{
					if (i != j)
					{
						float t0, t1;
						if (solids[j]->intersect(phit + nhit * bias, lightDirection, t0, t1) && (t0 > 0 || t1 > 0))
							//if(spheres[j].intersect(phit, lightDirection, t0, t1))
						{
							transmission = 0;
							break;
						}
					}
				}
				if (transmission.x != 0 || transmission.y != 0 || transmission.z != 0) {
					//hehe
					int zfefew = 0;
				}
				surfaceColor += solid->surfaceColor * 0.3;
				//If nhit and lightDirection's dot is less than 0, then no light.
				surfaceColor += solid->surfaceColor * transmission * std::max(float(0), nhit.dot(lightDirection)) * solids[i]->emissionColor;
			}
		}
	}

	return surfaceColor + solid->emissionColor;

}

//Render function, compute each pixel of the image.
void render(char *file, const std::vector<Solid*> &solids, const Vec_3f &offset, const float rotateAngle)
{
	unsigned width = 640, height = 480;
	Vec_3f *img = new Vec_3f[width * height], *pixel = img;
	float invWidth = 1 / float(width), invHeight = 1 / float(height);
	float fov = 60;
	float aspectratio = width / float(height);
	float angle = tan(M_PI * 0.5 * fov / 180.);
	//Trace all ray
	for (unsigned y = 0; y < height; y++)
	{
		for (unsigned x = 0; x < width; x++, pixel++)
		{
			float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
			float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			Vec_3f raydir(xx, yy, -1);
			rotateByZ(raydir, rotateAngle);
			raydir.normal();
			*pixel = trace(/*Vec_3f(0, 0, 10) + */offset, raydir, solids, 0);
		}
	}
	//Save the result
	std::ofstream ofs(file, std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned i = 0; i < width * height; i++)
	{
		//0,255
		ofs << (unsigned char)(std::min(float(1), img[i].x) * 255) <<
			(unsigned char)(std::min(float(1), img[i].y) * 255) <<
			(unsigned char)(std::min(float(1), img[i].z) * 255);
	}
	ofs.close();
	delete[] img;
}

void rotateByZ(Vec_3f &direction, float angle)
{
	float y = direction.y;
	float z = direction.z;
	direction.y = y * cos(angle / 180 * M_PI) + z * sin(angle / 180 * M_PI);
	direction.z = - y * sin(angle / 180 * M_PI) + z * cos(angle / 180 * M_PI);
}


