import { memo, ComponentPropsWithoutRef, forwardRef } from "react";
import {
  Field,
  Fieldset,
  Input,
  Label,
  Legend,
  Select,
  Button,
} from "@headlessui/react";
import { UseFormRenderOptionsResult } from "./FormRenderOptions.hook";

export interface FormRenderOptionsProps
  extends ComponentPropsWithoutRef<"form"> {
  hookResult: UseFormRenderOptionsResult;
}

const FormRenderOptions = memo(
  forwardRef<HTMLFormElement, FormRenderOptionsProps>(
    ({ hookResult, ...restProps }, ref) => {
      return (
        <form {...restProps} ref={ref}>
          <Fieldset className="border px-8 py-6 flex flex-col gap-6">
            <Legend className="text-lg font-semibold text-gray-800">
              Render Options
            </Legend>
            <Field>
              <Label className="block">Model</Label>
              <Select className="mt-1 block min-w-32" name="country">
                <option>Test</option>
              </Select>
            </Field>
            <Field>
              <Label className="block">Camera Position</Label>
              <div className="flex gap-2">
                <Input
                  className="w-16"
                  name="cameraX"
                  value={hookResult.cameraX}
                  onChange={hookResult.onCameraXChange}
                />
                <Input
                  className="w-16"
                  name="cameraY"
                  value={hookResult.cameraY}
                  onChange={hookResult.onCameraYChange}
                />
                <Input
                  className="w-16"
                  name="cameraZ"
                  value={hookResult.cameraZ}
                  onChange={hookResult.onCameraZChange}
                />
              </div>
            </Field>
            <Field>
              <Label className="block">Light Direction</Label>
              <div className="flex gap-2">
                <Input
                  className="w-16"
                  name="lightX"
                  value={hookResult.lightX}
                  onChange={hookResult.onLightXChange}
                />
                <Input
                  className="w-16"
                  name="lightY"
                  value={hookResult.lightY}
                  onChange={hookResult.onLightYChange}
                />
                <Input
                  className="w-16"
                  name="lightZ"
                  value={hookResult.lightZ}
                  onChange={hookResult.onLightZChange}
                />
              </div>
            </Field>
            <Button className="self-end mt-8" type="submit">
              Render Image
            </Button>
          </Fieldset>
        </form>
      );
    }
  )
);

export default FormRenderOptions;
