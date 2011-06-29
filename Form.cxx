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

#include "ui_Form.h"
#include "Form.h"

// Qt
#include <QFileDialog>
#include <QIcon>

// VTK
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkLine.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

// Constructor
Form::Form()
{
  this->setupUi(this);

  this->Renderer = vtkSmartPointer<vtkRenderer>::New();

  this->qvtkWidget->GetRenderWindow()->AddRenderer(this->Renderer);
  
  // Setup toolbar
  QIcon openIcon = QIcon::fromTheme("document-open");
  actionOpen->setIcon(openIcon);
  this->toolBar->addAction(actionOpen);
  
  QIcon saveIcon = QIcon::fromTheme("document-save");
  actionSave->setIcon(saveIcon);
  this->toolBar->addAction(actionSave);

};

// Define interaction style
class MyInteractorStyle : public vtkInteractorStyleTrackballActor
{
  public:
    static MyInteractorStyle* New();
    vtkTypeMacro(MyInteractorStyle, vtkInteractorStyleTrackballActor);
 
    MyInteractorStyle()
    {
      PreviousId = -1;
    }
    
    virtual void OnLeftButtonDown() 
    {
      // Forward events
      vtkInteractorStyleTrackballActor::OnLeftButtonDown();
      
      int selectedId = -1;
      for(unsigned int i = 0; i < this->Points.size(); ++i)
	{
	if(this->InteractionProp == this->Points[i].GetPointer())
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
 
    virtual void OnLeftButtonUp() 
    {
      // Forward events
      vtkInteractorStyleTrackballActor::OnLeftButtonUp();
    }
 
    //void SetPoints(std::vector<vtkActor*> points) {this->Points = points;}
    void SetPoints(std::vector<vtkSmartPointer<vtkActor> > points) {this->Points = points;}
    void SetContour(vtkSmartPointer<vtkPolyData> contour) {this->Contour = contour;}
 
  private:
    //std::vector<vtkActor*> Points;
    std::vector<vtkSmartPointer<vtkActor> > Points;
    int PreviousId;
    vtkSmartPointer<vtkPolyData> Contour;
};
vtkStandardNewMacro(MyInteractorStyle);


void Form::on_actionOpen_activated()
{
  // Get a filename to open
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", ".", "PolyData Files (*.vtp)");

  std::cout << "Got filename: " << fileName.toStdString() << std::endl;
  if(fileName.toStdString().empty())
    {
    std::cout << "Filename was empty." << std::endl;
    return;
    }

  this->Points.clear();
  this->PointActors.clear();
  this->PointMappers.clear();
  
  vtkSmartPointer<vtkXMLPolyDataReader> reader =
    vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(fileName.toStdString().c_str());
  reader->Update();

  for(unsigned int i = 0; i < reader->GetOutput()->GetNumberOfPoints(); ++i)
    {
    vtkSmartPointer<vtkSphereSource> sphereSource =
      vtkSmartPointer<vtkSphereSource>::New();
    double p[3];
    reader->GetOutput()->GetPoint(i,p);
    sphereSource->SetCenter(p);
    sphereSource->Update();
    this->Points.push_back(sphereSource->GetOutput());
  
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInput(this->Points[i]);
    this->PointMappers.push_back(mapper);
    
    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->PointActors.push_back(actor);
   
    }
    
  std::cout << "There are " << reader->GetOutput()->GetNumberOfPoints() << std::endl
	    << this->Points.size() << " polydata spheres" << std::endl
	    << this->PointMappers.size() << " mappers" << std::endl
	    << this->PointActors.size() << " actors" << std::endl;
    
  this->ContourActor = vtkSmartPointer<vtkActor>::New();
  this->ContourActor->GetProperty()->SetColor(1.0, 0.0, 0.0); // red
  this->ContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->Contour = vtkSmartPointer<vtkPolyData>::New();
  this->ContourMapper->SetInput(this->Contour);
  this->ContourActor->SetMapper(this->ContourMapper);
  this->Renderer->AddActor(this->ContourActor);
    
  this->Contour->SetPoints(reader->GetOutput()->GetPoints());
  this->Lines = vtkSmartPointer<vtkCellArray>::New();
  this->Contour->SetLines(this->Lines);
  
  // Add Actors to renderer
  for(unsigned int i = 0; i < this->PointActors.size(); ++i)
    {
    this->Renderer->AddActor(this->PointActors[i]);
    }
  this->Renderer->ResetCamera();
  this->Renderer->Render();
  this->qvtkWidget->GetRenderWindow()->Render();
  
  
  vtkSmartPointer<MyInteractorStyle> interactorStyle =
      vtkSmartPointer<MyInteractorStyle>::New();
  interactorStyle->SetPoints(this->PointActors);
  interactorStyle->SetContour(this->Contour);
  this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(interactorStyle);
  

}

void Form::on_actionSave_activated()
{

  QString fileName = QFileDialog::getSaveFileName(this, "Save File", ".", "Image Files (*.vtp)");
  std::cout << "Got filename: " << fileName.toStdString() << std::endl;
  if(fileName.toStdString().empty())
    {
    std::cout << "Filename was empty." << std::endl;
    return;
    }
  
  vtkSmartPointer<vtkXMLPolyDataWriter> writer =  
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(fileName.toStdString().c_str());
  writer->SetInput(this->Contour);
  writer->Write();

}
