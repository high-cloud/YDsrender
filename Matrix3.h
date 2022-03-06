#pragma once

#include"YDPrerequisites.h"
#include"Vector.h"

namespace YDSR
{

	class Matrix3
	{
	public:

		Matrix3() {}

		explicit Matrix3(const Real arr[3][3])
		{
            memcpy(m, arr, 9 * sizeof(Real));
		}

        Matrix3(Real fEntry00, Real fEntry01, Real fEntry02,
            Real fEntry10, Real fEntry11, Real fEntry12,
            Real fEntry20, Real fEntry21, Real fEntry22)
        {
            m[0][0] = fEntry00;
            m[0][1] = fEntry01;
            m[0][2] = fEntry02;
            m[1][0] = fEntry10;
            m[1][1] = fEntry11;
            m[1][2] = fEntry12;
            m[2][0] = fEntry20;
            m[2][1] = fEntry21;
            m[2][2] = fEntry22;
        }


        /// Member access, allows use of construct mat[r][c]
        const Real* operator[] (size_t iRow) const
        {
            return m[iRow];
        }

        Real* operator[] (size_t iRow)
        {
            return m[iRow];
        }

        Vector3 GetColumn(size_t iCol) const
        {
            assert(iCol < 3);
            return Vector3(m[0][iCol], m[1][iCol], m[2][iCol]);
        }
        void SetColumn(size_t iCol, const Vector3& vec)
        {
            assert(iCol < 3);
            m[0][iCol] = vec.x;
            m[1][iCol] = vec.y;
            m[2][iCol] = vec.z;
        }

        /** Tests 2 matrices for equality.
 */
        bool operator== (const Matrix3& rkMatrix) const;

        /** Tests 2 matrices for inequality.
         */
        bool operator!= (const Matrix3& rkMatrix) const
        {
            return !operator==(rkMatrix);
        }

	private:
        Real m[3][3] = {};
	};

}
