import { memo } from "react";
import {
  Field,
  Fieldset,
  FieldsetProps,
  Input,
  Label,
  Legend,
  Select,
  Button,
} from "@headlessui/react";
import clsx from "clsx";

export interface FieldSetRenderOptionsProps extends FieldsetProps {}

const FieldSetRenderOptions = memo(
  ({ className, ...restProps }: FieldSetRenderOptionsProps) => {
    return (
      <Fieldset
        className={clsx("border px-8 py-6 flex flex-col gap-6", className)}
        {...restProps}
      >
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
            <Input className="w-16" name="cameraX" />
            <Input className="w-16" name="cameraY" />
            <Input className="w-16" name="cameraZ" />
          </div>
        </Field>
        <Field>
          <Label className="block">Light Direction</Label>
          <div className="flex gap-2">
            <Input className="w-16" name="cameraX" />
            <Input className="w-16" name="cameraY" />
            <Input className="w-16" name="cameraZ" />
          </div>
        </Field>
        <Button className="self-end mt-8">Render Image</Button>
      </Fieldset>
    );
  }
);

export default FieldSetRenderOptions;
