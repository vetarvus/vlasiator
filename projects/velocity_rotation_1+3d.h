#include "definitions.h"

/** Query if spatial cell parameters (of any cell) have changed and need to be 
 * recalculated. If you have a completely static case, then you can always return 
 * false here. Otherwise you need to return true so that function calcCellParameters
 * gets called for each spatial cell.
 * @param t The current value of time.
 * @return If true, the spatial cell parameters need to be recalculated.
 */
bool cellParametersChanged(creal& t);

/** Calculate parameters for the given velocity grid block.
 * Here you need to set a value for array indices: BlockParams::Q_PER_M.
 * @param blockParams Array containing the block parameters
 */
void calcBlockParameters(Real* blockParams);

/** Calculate parameters for the given spatial cell at the given time.
 * Here you need to set values for the following array indices:
 * CellParams::EX, CellParams::EY, CellParams::EZ, CellParams::BX, CellParams::BY, and CellParams::BZ.
 * 
 * The following array indices contain the coordinates of the "lower left corner" of the cell: 
 * CellParams::XCRD, CellParams::YCRD, and CellParams::ZCRD.
 * The cell size is given in the following array indices: CellParams::DX, CellParams::DY, and CellParams::DZ.
 * @param cellParams Array containing cell parameters.
 * @param t The current value of time. This is passed as a convenience. If you need more detailed information 
 * of the state of the simulation, you can read it from Parameters.
 */
void calcCellParameters(Real* cellParams,creal& t);

/** Integrate the distribution function over the given six-dimensional phase-space cell.
 * @param x Starting value of the x-coordinate of the cell.
 * @param y Starting value of the y-coordinate of the cell.
 * @param z Starting value of the z-coordinate of the cell.
 * @param dx The size of the cell in x-direction.
 * @param dy The size of the cell in y-direction.
 * @param dz The size of the cell in z-direction.
 * @param vx Starting value of the vx-coordinate of the cell.
 * @param vy Starting value of the vy-coordinate of the cell.
 * @param vz Starting value of the vz-coordinate of the cell.
 * @param dvx The size of the cell in vx-direction.
 * @param dvy The size of the cell in vy-direction.
 * @param dvz The size of the cell in vz-direction.
 * @return The integral of the distribution function over the given six-dimensional phase-space cell.
 */
Real calcPhaseSpaceDensity(creal& x,creal& y,creal& z,creal& dx,creal& dy,creal& dz,
			   creal& vx,creal& vy,creal& vz,creal& dvx,creal& dvy,creal& dvz);

template<typename T> T spatialFluxX(cuint& i,const T& avg_neg,const T& avg_pos,const T* const blockParams) {
   return 0.0;
}

template<typename T> T spatialFluxY(cuint& j,const T& avg_neg,const T& avg_pos,const T* const blockParams) {
   return 0.0;
}

template<typename T> T spatialFluxZ(cuint& k,const T& avg_neg,const T& avg_pos,const T* const blockParams) {
   return 0.0;
}

template<typename T> T velocityFluxX(const T& j,const T& k,const T& avg_neg,const T& avg_pos,const T* const cellParams,const T* const blockParams) {
   const T VY = blockParams[BlockParams::VYCRD] + (j+convert<T>(0.5))*blockParams[BlockParams::DVY];
   const T VZ = blockParams[BlockParams::VZCRD] + (k+convert<T>(0.5))*blockParams[BlockParams::DVZ];
   const T EX = cellParams[CellParams::EX];
   const T BY = cellParams[CellParams::BY];
   const T BZ = cellParams[CellParams::BZ];
   const T AX = blockParams[BlockParams::Q_PER_M]*(EX + VY*BZ - VZ*BY);
   return convert<T>(0.5)*AX*(avg_neg + avg_pos) - convert<T>(0.5)*fabs(AX)*(avg_pos-avg_neg);
}

template<typename T> T velocityFluxY(const T& i,const T& k,const T& avg_neg,const T& avg_pos,const T* const cellParams,const T* const blockParams) {
   const T VX = blockParams[BlockParams::VXCRD] + (i+convert<T>(0.5))*blockParams[BlockParams::DVX];
   const T VZ = blockParams[BlockParams::VZCRD] + (k+convert<T>(0.5))*blockParams[BlockParams::DVZ];
   const T EY = cellParams[CellParams::EY];
   const T BX = cellParams[CellParams::BX];
   const T BZ = cellParams[CellParams::BZ];
   
   const T AY = blockParams[BlockParams::Q_PER_M]*(EY + VZ*BX - VX*BZ);
   return convert<T>(0.5)*AY*(avg_neg + avg_pos) - convert<T>(0.5)*fabs(AY)*(avg_pos-avg_neg);
}

template<typename T> T velocityFluxZ(const T& i,const T& j,const T& avg_neg,const T& avg_pos,const T* const cellParams,const T* const blockParams) {
   const T VX = blockParams[BlockParams::VXCRD] + (i+convert<T>(0.5))*blockParams[BlockParams::DVX];
   const T VY = blockParams[BlockParams::VYCRD] + (j+convert<T>(0.5))*blockParams[BlockParams::DVY];
   const T EZ = cellParams[CellParams::EZ];
   const T BX = cellParams[CellParams::BX];
   const T BY = cellParams[CellParams::BY];
   
   const T AZ = blockParams[BlockParams::Q_PER_M]*(EZ + VX*BY - VY*BX);
   return convert<T>(0.5)*AZ*(avg_neg + avg_pos) - convert<T>(0.5)*fabs(AZ)*(avg_pos-avg_neg);
}

