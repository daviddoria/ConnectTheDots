/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef INTERACTORSTYLE_H
#define INTERACTORSTYLE_H

#include <vtkInteractorStyleTrackballActor.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPolyData.h>

#include <vector>

class InteractorStyle : public vtkInteractorStyleTrackballActor
{
  public:
    static InteractorStyle* New();
    vtkTypeMacro(InteractorStyle, vtkInteractorStyleTrackballActor);
 
    InteractorStyle();
    
    virtual void OnLeftButtonDown();
 
    virtual void OnLeftButtonUp();
 
    void SetPoints(std::vector<vtkSmartPointer<vtkActor> >* points);
    void SetContour(vtkSmartPointer<vtkPolyData> contour);
 
  private:
    std::vector<vtkSmartPointer<vtkActor> >* Points;
    int PreviousId;
    vtkSmartPointer<vtkPolyData> Contour;
};

#endif
