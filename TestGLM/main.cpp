/*
Date: 2019-1-31 8:20
Author: lichongbin
Email: lichongbin2004@126.com
Description:
     I made a mistake at the first time of using glm::translate and glm::scale.
	 I want to locate an object in the world space, the first thing which 
	 should be done is scale and translate.	we often scale the object at first,
	 then translate it. Suppose the scaling and translating operation is denoted
	 by two matrices named MS and MT, the final matrix is MT * MS which is what I
	 want.
	 But i call glm::scale at first, then glm::translate, the final matrix
	 will be MS * MT which is wrong.
	 The following code snippet tests this logic exactly.
*/

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <iomanip>

using namespace std;

void ShowMat4(glm::mat4 &m)
{
	glm::mat4 t = glm::transpose(m);
	cout << "Matrix:\n";
	for (size_t r = 0; r < 4; r++)
	{
		for (size_t c = 0; c < 4; c++)
		{
			cout << setw(6) << t[r][c] << ",";
		}
		cout << endl;
	}
	cout << endl;
}

int main()
{
	// 1.translate  2.scale
	glm::mat4 model = glm::mat4(1.0f);
	ShowMat4(model);

	model = glm::translate(model, glm::vec3(1, 2, 3));
	ShowMat4(model);

	model = glm::scale(model, glm::vec3(0.5, 0.5, 1.0));
	ShowMat4(model);

	// 1.scale  2.translate
	glm::mat4 model2 = glm::mat4(1.0f);
	ShowMat4(model2);

	model2 = glm::scale(model2, glm::vec3(0.5, 0.5, 1.0));
	ShowMat4(model2);

	model2 = glm::translate(model2, glm::vec3(1, 2, 3));
	ShowMat4(model2);

	int i;
	cin >> i;

	return 0;
}