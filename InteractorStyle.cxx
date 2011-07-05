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

#include "InteractorStyle.h"

#include <vtkObjectFactory.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(InteractorStyle);

InteractorStyle::InteractorStyle()
{
  PreviousId = -1;
}

void InteractorStyle::OnLeftButtonDown() 
{
  // Forward events
  vtkInteractorStyleTrackballActor::OnLeftButtonDown();
  
  int selectedId = -1;
  for(unsigned int i = 0; i < this->Points->size(); ++i)
    {
    if(this->InteractionProp == this->Points->at(i).GetPointer())
      {
      //std::cout << "Point " << i << " was selected." << std::endl;
      selectedId = i;
      break;
      }
    }

  if(selectedId == -1)
    {
    return;
    }

  if(this->PreviousId == -1)
    {
    this->PreviousId = selectedId;
    return;
    }
    
  //std::cout << "Previous: " << this->PreviousId << " Current: " << selectedId << std::endl;
  
  // At this point, we should have two valid points to connect
  vtkSmartPointer<vtkLine> line =
    vtkSmartPointer<vtkLine>::New();
  line->GetPointIds()->SetId(0,this->PreviousId);
  line->GetPointIds()->SetId(1,selectedId);
  this->Contour->GetLines()->InsertNextCell(line);
  this->Contour->Modified();
  
  //this->Interactor->GetRenderWindow()->GetRenderers()->Render();
  this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->Render();
  this->Interactor->GetRenderWindow()->Render();

  std::cout << "There are now " << this->Contour->GetNumberOfLines() << " lines." << std::endl;
  
  this->PreviousId = selectedId;
}

void InteractorStyle::OnLeftButtonUp() 
{
  // Forward events
  vtkInteractorStyleTrackballActor::OnLeftButtonUp();
}

//void SetPoints(std::vector<vtkActor*> points) {this->Points = points;}
void InteractorStyle::SetPoints(std::vector<vtkSmartPointer<vtkActor> >* points)
{
  this->Points = points;
}

void InteractorStyle::SetContour(vtkSmartPointer<vtkPolyData> contour)
{
  this->Contour = contour;
}
